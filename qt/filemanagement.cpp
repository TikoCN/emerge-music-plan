#include "filemanagement.h"
#include "base.h"
#include "ffmpeg.h"
#include "datacore/dataactive.h"

FileManagement::FileManagement() {}

void FileManagement::writeMusicToFile(QStringList key, QStringList value, int musicId)
{
    DataActive *core = DataActive::getInstance();
    MusicPtr music = core->getMusicCore(musicId);
    if (music == NULL)
        return Base::getInstance()->sendMessage(QObject::tr("写入歌曲信息失败"), 1);;

    QString url = music->url;

    QString baseName = Base::getInstance()->getFileName(url);
    QString newUrl = url.replace(baseName, "new"+baseName);
    FFmpeg ff;
    if(ff.writeDict(key, value, url, newUrl)){
        Base::getInstance()->writeFileText(url, newUrl);
    }
    else{
        Base::getInstance()->sendMessage(QObject::tr("获取歌曲信息失败"), 1);
    }
}

QString FileManagement::getMusicLrcUrl(int musicId)
{
    DataActive *core = DataActive::getInstance();
    MusicPtr music = core->getMusicCore(musicId);

    if (music == NULL) {
        Base::getInstance()->sendMessage(QObject::tr("获取歌曲信息失败"), 1);;
        return "";
    }

    QString url = music->url;

    QString lrc = url.split("." + url.split(".").last())[0];
    QString hlrc = lrc + ".hlrc";

    if(QFile::exists(hlrc)){
        lrc = hlrc;
    }
    else {
        lrc += ".lrc";
    }
    return lrc;
}

QString FileManagement::getMusicLrcData(int musicId)
{
    QString lrc = getMusicLrcUrl(musicId);
    QFile lrcFile(lrc);
    if(!lrcFile.open(QIODevice::Text |QIODevice::ReadOnly)){
        return QString();
    }
    QTextStream out(&lrcFile);
    QString lrcData = out.readAll();
    lrcFile.close();

    return lrcData;
}

void FileManagement::wrtiLrcData(int musicId, QString lrcData)
{
    QString lrc = getMusicLrcUrl(musicId);
    QFile lrcFile(lrc);
    if(!lrcFile.open(QIODevice::Text |QIODevice::WriteOnly)){
        return;
    }
    QTextStream in(&lrcFile);
    in << lrcData;
    lrcFile.close();
}

QList<LrcDataPtr > FileManagement::getMusicLyricsData(int musicId)
{
    TLog::getInstance()->logError(tr("开始读取歌曲歌词") +
                                  tr("歌曲ID:%1").arg(musicId));

    QString lrc = getMusicLrcUrl(musicId);
    QList<LrcDataPtr > lrcList;

    DataActive *core = DataActive::getInstance();
    MusicPtr music = core->getMusicCore(musicId);
    if (music == nullptr) {
        TLog::getInstance()->logError(tr("获取数据失败") +
                                      tr("歌曲ID:%1").arg(musicId));
        return lrcList;
    }

    QFile lrcFile(lrc);
    if(!lrcFile.open(QIODevice::Text |QIODevice::ReadOnly)){
        return lrcList;
    }

    QTextStream in(&lrcFile);
    QRegularExpression rx;
    QRegularExpressionMatch match;
    QString line;
    LrcDataPtr lrcD;

    //读取高级歌词
    if(lrc.split(".").last() == "hlrc"){
        while(!in.atEnd()){
            line = in.readLine();

            //捕获开始时间和结束时间
            rx.setPattern(R"(\[(\d+),(\d+)\])");
            match = rx.match(line);
            //初始化并设置开始结束时间
            if(match.isValid()){
                lrcD = LrcDataPtr(new LrcData);
                lrcList.append(lrcD);
                lrcD->id = lrcList.size()-1;
                lrcD->startTime = match.captured(1).toLong();
                lrcD->endTime = match.captured(2).toLong();
            }
            else{//没有发现行头，下一行
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
            while(it.hasNext()){
                match = it.next();
                long long start = match.captured(1).toLong();
                long long end = match.captured(2).toLong();
                QString text = match.captured(3);
                lrcD->append(start, end, text);
            }
        }
    }
    else{
        rx.setPattern(R"(\[(\d+):(\d+).(\d+)\]([\s\S]*))");
        QStringList lrcTextList;
        //读取基本数据以及文本行
        while (!in.atEnd()) {
            line = in.readLine();
            match = rx.match(line);
            if(match.hasMatch()){
                lrcD = LrcDataPtr(new LrcData);
                lrcD->id = lrcList.size();
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
            }
            else{
                continue;
            }
        }

        //设置逐字时间戳
        for(int i=0; i<lrcList.size(); i++){
            long long start = lrcList[i]->startTime;
            long long end;
            if(i == lrcList.size() - 1){
                end = music->duration;
            }
            else{
                end = lrcList[i+1]->startTime;
            }

            lrcList[i]->endTime = end;
            int length = lrcTextList[i].size() == 0 ? 1 : lrcTextList[i].size();
            int wordTime = (end - start) / length;

            //逐字遍历
            QString text = lrcTextList[i];
            for(int j=0; j<text.size(); j++){
                lrcList[i]->append(start + j * wordTime,
                                   start + (j + 1) * wordTime,
                                   text[j]);
            }
        }
    }

    lrcFile.close();
    return lrcList;
}

QJsonArray FileManagement::getMusicAllTaglib(int musicId)
{
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

void FileManagement::openPlayListDir(int playListId)
{

}
