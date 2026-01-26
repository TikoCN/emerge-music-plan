#include "filemanagement.h"
#include "ffmpeg.h"
#include "datacore/dataactive.h"
#include <QDir>
#include <QDesktopServices>
#include "baseclass/dataexception.h"
#include <QJsonObject>

FileManagement::FileManagement() = default;

void FileManagement::writeMusicToFile(const QStringList &key, const QStringList &value, const int musicId) {
    DataActive *core = DataActive::getInstance();
    const MusicPtr music = core->getMusicCore(musicId);
    if (music == nullptr)
        return;

    QString url = music->url;

    const QString baseName = getFileName(url);
    const QString newUrl = url.replace(baseName, "new" + baseName);
    if (FFmpeg ff; ff.writeDict(key, value, url, newUrl)) {
        if (writeFileText(url, newUrl)) {
            TLog::getInstance()->logError(tr("元数据写入文件音乐失败"));
        }
    }
}

QString FileManagement::getMusicLrcUrl(const int musicId) {
    DataActive *core = DataActive::getInstance();
    const MusicPtr music = core->getMusicCore(musicId);

    if (music == nullptr) {
        return "";
    }

    const QString url = music->url;

    QString lrc = url.split("." + url.split(".").last())[0];
    QString hlrc = lrc + ".hlrc";

    if (QFile::exists(hlrc)) {
        lrc = hlrc;
    } else {
        lrc += ".lrc";
    }
    return lrc;
}

QString FileManagement::getMusicLrcData(const int musicId) {
    const QString lrc = getMusicLrcUrl(musicId);
    QFile lrcFile(lrc);
    if (!lrcFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        return {};
    }
    QTextStream out(&lrcFile);
    QString lrcData = out.readAll();
    lrcFile.close();

    return lrcData;
}

void FileManagement::wrtiLrcData(const int musicId, const QString &lrcData) {
    QString lrc = getMusicLrcUrl(musicId);
    QFile lrcFile(lrc);
    if (!lrcFile.open(QIODevice::Text | QIODevice::WriteOnly)) {
        return;
    }
    QTextStream in(&lrcFile);
    in << lrcData;
    lrcFile.close();
}

QList<LrcDataPtr> FileManagement::getMusicLyricsData(const int musicId) {
    TLog::getInstance()->logError(tr("开始读取歌曲歌词") +
                                  tr("歌曲ID:%1").arg(musicId));

    QString lrc = getMusicLrcUrl(musicId);
    QList<LrcDataPtr> lrcList;

    DataActive *core = DataActive::getInstance();
    MusicPtr music = core->getMusicCore(musicId);
    if (music == nullptr) {
        TLog::getInstance()->logError(tr("获取数据失败") +
                                      tr("歌曲ID:%1").arg(musicId));
        return lrcList;
    }

    QFile lrcFile(lrc);
    if (!lrcFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        return lrcList;
    }

    QTextStream in(&lrcFile);
    QRegularExpression rx;
    QRegularExpressionMatch match;
    QString line;
    LrcDataPtr lrcD;

    //读取高级歌词
    if (lrc.split(".").last() == "hlrc") {
        while (!in.atEnd()) {
            line = in.readLine();

            //捕获开始时间和结束时间
            rx.setPattern(R"(\[(\d+),(\d+)\])");
            match = rx.match(line);
            //初始化并设置开始结束时间
            if (match.isValid()) {
                lrcD = LrcDataPtr(new LrcData);
                lrcList.append(lrcD);
                lrcD->id = static_cast<int>(lrcList.size()) - 1;
                lrcD->startTime = match.captured(1).toLong();
                lrcD->endTime = match.captured(2).toLong();
            } else {
                //没有发现行头，下一行
                continue;
            }

            //捕获主体
            QStringList lrcText = line.split("/");
            // 添加到其他文本
            for (int i = 1; i < lrcText.size(); ++i) {
                lrcD->helpTextList.append(lrcText[i]);
            }

            rx.setPattern(R"(\((\d+),(\d+)\)\s*([^(]*))");
            QRegularExpressionMatchIterator it = rx.globalMatch(lrcText.first());
            while (it.hasNext()) {
                match = it.next();
                long long start = match.captured(1).toLong();
                long long end = match.captured(2).toLong();
                QString text = match.captured(3);
                lrcD->append(start, end, text);
            }
        }
    } else {
        rx.setPattern(R"(\[(\d+):(\d+).(\d+)\]([\s\S]*))");
        QStringList lrcTextList;
        //读取基本数据以及文本行
        while (!in.atEnd()) {
            line = in.readLine();
            match = rx.match(line);
            if (match.hasMatch()) {
                lrcD = LrcDataPtr(new LrcData);
                lrcD->id = static_cast<int>(lrcList.size());
                lrcD->startTime = match.captured(1).toLong() * 60 * 1000 +
                                  match.captured(2).toLong() * 1000 +
                                  match.captured(3).toLong();
                QStringList lrcText = match.captured(4).split("/");
                // 添加到其他文本
                for (int i = 1; i < lrcText.size(); ++i) {
                    lrcD->helpTextList.append(lrcText[i]);
                }

                lrcTextList.append(lrcText.first());
                lrcList.append(lrcD);
            } else {
                continue;
            }
        }

        //设置逐字时间戳
        for (int i = 0; i < lrcList.size(); i++) {
            long long start = lrcList[i]->startTime;
            long long end;
            if (i == lrcList.size() - 1) {
                end = music->duration;
            } else {
                end = lrcList[i + 1]->startTime;
            }

            lrcList[i]->endTime = end;
            const long long length = lrcTextList[i].size() == 0 ? 1 : lrcTextList[i].size();
            const long long wordTime = (end - start) / length;

            //逐字遍历
            QString text = lrcTextList[i];
            for (int j = 0; j < text.size(); j++) {
                lrcList[i]->append(start + j * wordTime,
                                   start + (j + 1) * wordTime,
                                   text[j]);
            }
        }
    }

    lrcFile.close();
    return lrcList;
}

QJsonArray FileManagement::getMusicAllTaglib(int musicId) {
    DataActive *core = DataActive::getInstance();
    MusicPtr music = core->getMusicCore(musicId);

    QStringList key, value;
    FFmpeg ff;
    ff.getDict(&key, &value, music->url);

    QJsonArray array;
    for (int i = 0; i < key.length(); ++i) {
        QJsonObject json;
        json.insert("key", key[i]);
        json.insert("value", value[i]);
        array.append(json);
    }

    return array;
}

void FileManagement::openPlayListDir(int playListId) {
}

QString FileManagement::getArtistCoverUrl(const QString &name) {
    return QDir::currentPath() + "/artist/" + name + ".jpg";
}

QString FileManagement::getAlbumCoverUrl(const QString &name) {
    return QDir::currentPath() + "/artist/" + name + ".jpg";
}

void FileManagement::replaceFile(const QString &inUrl, const QString &outUrl) {
    bool r = false;
    QFile file(inUrl);
    if (file.exists()) {
        r = file.remove();
        if (!r) {
            //sendMessage(inUrl + tr("无法删除"), 1);
        }
        return;
    }
    file.close();

    QFile out(outUrl);
    if (out.exists()) {
        r = out.rename(inUrl);
        if (!r) {
            //sendMessage(outUrl + tr("重命名"), 1);
        }
    }
    out.close();
}

QString FileManagement::readFileText(const QString &url) {
    QString data;
    QFile file(url);
    if (!file.exists()) {
        //todo sendMessage(url + tr(" 文件不存在"),1);
        return data;
    }

    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        //todo sendMessage(url + tr(" 打开文件失败"),1);
        return data;
    }

    QTextStream in(&file);
    data = in.readAll();
    file.close();
    return data;
}

bool FileManagement::writeFileText(const QString &url, const QString &data) {
    QFile file(url);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
        //sendMessage(url + tr(" 打开文件失败"),1);
        return false;
    }

    QTextStream in(&file);
    in << data;
    file.close();
    return true;
}

void FileManagement::deskOpenFile(QString url, const bool local) {
    //检验本地文件
    if (!url.contains("file:///") && local) {
        url = "file:///" + url;
    }

    QDesktopServices::openUrl(QUrl(url));
}

bool FileManagement::renameFile(const QString &oldUrl, const QString &newUrl) {
    try {
        QFile oldFile(oldUrl);
        QFile newFile(newUrl);

        if (!oldFile.exists()) {
            throw DataException(oldUrl + tr("文件不存在"));
        }

        if (!oldFile.setPermissions(QFile::WriteOther)) {
            throw DataException(oldUrl + "获取权限失败," + oldFile.errorString());
        }

        // 移除以及存在文件
        if (newFile.exists()) {
            if (!newFile.setPermissions(QFile::WriteOther)) {
                throw DataException(newUrl + "获取权限失败," + newFile.errorString());
            }

            if (!newFile.remove()) {
                throw DataException(newUrl + tr("文件已经存在且无法删除,") + newFile.errorString());
            }
        }

        if (!QFile::rename(oldUrl, newUrl)) {
            throw DataException(oldUrl + tr("重命名失败,") + oldFile.errorString());
        }
    } catch (const DataException &e) {
        TLog::getInstance()->logError(e.errorMessage());
        return false;
    }
    return true;
}

QString FileManagement::getBaseUrl(const QString &url) {
    return url.split("." + url.split(".").last()).first();
}

QString FileManagement::getFileName(const QString &url) {
    QString FileManagementUrl = getBaseUrl(url);
    return FileManagementUrl.split("/").last();
}

QString FileManagement::getParentDir(const QString &url) {
    return url.split("/" + url.split("/").last()).last();
}
