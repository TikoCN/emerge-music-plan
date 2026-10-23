#include "OnLine.h"

#include "Setting.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QMetaObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSaveFile>
#include <QThreadPool>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QRunnable>
#include <functional>

namespace {

/**
 * @brief 在当前工作线程中获取网络响应。
 * @param url 请求地址
 * @return 成功时返回响应内容，失败或超时时返回空数据
 */
QByteArray fetchUrl(const QUrl &url) {
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setTransferTimeout(15000);
    QNetworkReply *pReply = manager.get(request);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    QObject::connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, pReply, &QNetworkReply::abort);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(15000);
    if (!pReply->isFinished()) {
        loop.exec();
    }

    const bool isSuccess = pReply->error() == QNetworkReply::NoError
                           && pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() >= 200
                           && pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() < 300;
    const QByteArray data = isSuccess ? pReply->readAll() : QByteArray();
    delete pReply;
    return data;
}

/**
 * @brief 将下载内容安全写入目标文件。
 * @param data 文件内容
 * @param filePath 目标文件路径
 * @return 写入成功时返回 true
 */
bool saveFile(const QByteArray &data, const QString &filePath) {
    if (data.isEmpty()) {
        return false;
    }

    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    if (file.write(data) != data.size()) {
        file.cancelWriting();
        return false;
    }
    return file.commit();
}

/**
 * @brief 从匹配表达式中提取首个捕获组。
 * @param data 响应内容
 * @param pattern 正则表达式
 * @return 捕获内容；未匹配时返回空字符串
 */
QString captureFirst(const QByteArray &data, const QString &pattern) {
    const QRegularExpression expression(pattern);
    const QRegularExpressionMatch match = expression.match(QString::fromUtf8(data));
    return match.hasMatch() ? match.captured(1) : QString();
}

/**
 * @brief 从服务端 JSON 响应中提取歌词文本。
 * @param data 响应内容
 * @param isNetEase 是否为网易云音乐响应
 * @return 歌词文本；响应无效或没有歌词时返回空字符串
 */
QString extractLyrics(const QByteArray &data, const bool isNetEase) {
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return {};
    }

    const QJsonObject response = document.object();
    const QString lyrics = isNetEase
                               ? response.value("lrc").toObject().value("lyric").toString()
                               : response.value("lyric").toString();
    return lyrics.trimmed();
}

/**
 * @brief 查询 QQ 音乐并获取歌曲封面。
 * @param key 歌曲搜索词
 * @return 获取到的封面数据；失败时返回空数据
 */
QByteArray downloadQQMusicCover(const QString &key) {
    QUrl searchUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp");
    QUrlQuery query;
    query.addQueryItem("p", "1");
    query.addQueryItem("n", "2");
    query.addQueryItem("w", key);
    query.addQueryItem("format", "json");
    searchUrl.setQuery(query);

    const QString albumId = captureFirst(fetchUrl(searchUrl), "\\\"albummid\\\":\\\"([^\\\"]*)\\\"");
    if (albumId.isEmpty()) {
        return {};
    }

    const QUrl coverUrl("https://y.gtimg.cn/music/photo_new/T002R180x180M000" + albumId + ".jpg");
    return fetchUrl(coverUrl);
}

/**
 * @brief 查询网易云音乐并获取歌曲封面。
 * @param key 歌曲搜索词
 * @return 获取到的封面数据；失败时返回空数据
 */
QByteArray downloadNetEaseCover(const QString &key) {
    QUrl searchUrl("https://music.163.com/api/search/get/web");
    QUrlQuery query;
    query.addQueryItem("csrf_token", "");
    query.addQueryItem("s", key);
    query.addQueryItem("type", "1");
    query.addQueryItem("offset", "0");
    query.addQueryItem("total", "true");
    query.addQueryItem("limit", "1");
    searchUrl.setQuery(query);

    const QString musicId = captureFirst(fetchUrl(searchUrl), "\\\"id\\\":([^,]*),");
    if (musicId.isEmpty()) {
        return {};
    }

    QUrl detailUrl("https://music.163.com/api/song/detail/");
    QUrlQuery detailQuery;
    detailQuery.addQueryItem("id", musicId);
    detailQuery.addQueryItem("ids", "[" + musicId + "]");
    detailUrl.setQuery(detailQuery);
    const QString imageUrl = captureFirst(fetchUrl(detailUrl), "\\\"blurPicUrl\\\":\\\"([^\\\"]*)\\\"");
    return imageUrl.isEmpty() ? QByteArray() : fetchUrl(QUrl(imageUrl));
}

/**
 * @brief 从 Bing 图片搜索结果获取封面。
 * @param key 歌曲搜索词
 * @return 获取到的封面数据；失败时返回空数据
 */
QByteArray downloadBingCover(const QString &key) {
    QUrl searchUrl("https://www.bing.com/images/async");
    QUrlQuery query;
    query.addQueryItem("q", key);
    query.addQueryItem("first", "1");
    query.addQueryItem("count", "10");
    query.addQueryItem("mmasync", "1");
    searchUrl.setQuery(query);

    const QString imageUrl = captureFirst(fetchUrl(searchUrl), "src=\\\"([^\\\"]*)\\\"");
    return imageUrl.isEmpty() ? QByteArray() : fetchUrl(QUrl(imageUrl));
}

/**
 * @brief 从百度图片搜索结果获取封面。
 * @param key 歌曲搜索词
 * @return 获取到的封面数据；失败时返回空数据
 */
QByteArray downloadBaiduCover(const QString &key) {
    QUrl searchUrl("https://image.baidu.com/search/acjson");
    QUrlQuery query;
    query.addQueryItem("tn", "resultjson_com");
    query.addQueryItem("ipn", "rj");
    query.addQueryItem("word", key);
    query.addQueryItem("queryWord", key);
    query.addQueryItem("pn", "1");
    query.addQueryItem("rn", "10");
    searchUrl.setQuery(query);

    const QString imageUrl = captureFirst(fetchUrl(searchUrl), "\\\"thumbURL\\\":\\\"([^\\\"]*)\\\"");
    return imageUrl.isEmpty() ? QByteArray() : fetchUrl(QUrl(imageUrl));
}

/**
 * @brief 查询 QQ 音乐并获取歌词。
 * @param key 歌曲搜索词
 * @return 获取到的歌词数据；失败时返回空数据
 */
QByteArray downloadQQMusicLyrics(const QString &key) {
    QUrl searchUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp");
    QUrlQuery query;
    query.addQueryItem("p", "1");
    query.addQueryItem("n", "2");
    query.addQueryItem("w", key);
    query.addQueryItem("format", "json");
    searchUrl.setQuery(query);

    const QString songId = captureFirst(fetchUrl(searchUrl), "\\\"songmid\\\":\\\"([^\\\"]*)\\\"");
    if (songId.isEmpty()) {
        return {};
    }

    QUrl lyricsUrl("https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg");
    QUrlQuery lyricsQuery;
    lyricsQuery.addQueryItem("songmid", songId);
    lyricsQuery.addQueryItem("format", "json");
    lyricsQuery.addQueryItem("nobase64", "1");
    lyricsUrl.setQuery(lyricsQuery);

    const QString lyrics = extractLyrics(fetchUrl(lyricsUrl), false);
    return lyrics.toUtf8();
}

/**
 * @brief 查询网易云音乐并获取歌词。
 * @param key 歌曲搜索词
 * @return 获取到的歌词数据；失败时返回空数据
 */
QByteArray downloadNetEaseLyrics(const QString &key) {
    QUrl searchUrl("https://music.163.com/api/search/get/web");
    QUrlQuery query;
    query.addQueryItem("csrf_token", "");
    query.addQueryItem("s", key);
    query.addQueryItem("type", "1");
    query.addQueryItem("offset", "0");
    query.addQueryItem("total", "true");
    query.addQueryItem("limit", "1");
    searchUrl.setQuery(query);

    const QString musicId = captureFirst(fetchUrl(searchUrl), "\\\"id\\\":([^,]*),");
    if (musicId.isEmpty()) {
        return {};
    }

    QUrl lyricsUrl("https://music.163.com/api/song/lyric");
    QUrlQuery lyricsQuery;
    lyricsQuery.addQueryItem("id", musicId);
    lyricsQuery.addQueryItem("lv", "1");
    lyricsQuery.addQueryItem("kv", "1");
    lyricsQuery.addQueryItem("tv", "-1");
    lyricsUrl.setQuery(lyricsQuery);

    const QString lyrics = extractLyrics(fetchUrl(lyricsUrl), true);
    return lyrics.toUtf8();
}

void showResourcePreview(OnLine *pOnLine, const QString &filePath, const QByteArray &data, const bool isImage) {
    if (data.isEmpty()) {
        QMetaObject::invokeMethod(pOnLine, [pOnLine]() {
            emit pOnLine->resourceDownloadFailed(
                QCoreApplication::translate("OnLine", "未能获取网络资源，请检查网络连接或搜索条件。"));
        }, Qt::QueuedConnection);
        return;
    }

    QString oldResourceUrl;
    if (isImage) {
        oldResourceUrl = QFile::exists(filePath) ? QUrl::fromLocalFile(filePath).toString() : QString();
    } else if (QFile::exists(filePath)) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            oldResourceUrl = QString::fromUtf8(file.readAll());
        }
    }
    const QString newResourceData = isImage ? QString::fromLatin1(data.toBase64()) : QString::fromUtf8(data);
    QMetaObject::invokeMethod(pOnLine, [pOnLine, oldResourceUrl, newResourceData, filePath, isImage]() {
        emit pOnLine->resourcePreviewReady(oldResourceUrl, newResourceData, filePath, isImage);
    }, Qt::QueuedConnection);
}

void runAsync(const std::function<void()> &task) {
    QThreadPool::globalInstance()->start(QRunnable::create(task));
}

}  // namespace

OnLine::OnLine() = default;

void OnLine::downMusicCover(const QString &key, const QString &url) {
    const Setting *pSetting = &Setting::getInstance();
    if (!pSetting->getIsOnLine()) {
        return;
    }

    const bool isQQEnabled = pSetting->getIsGetCoverFromQQMusic();
    const bool isNetEaseEnabled = pSetting->getIsGetCoverFromNetEase();
    const bool isBaiduEnabled = pSetting->getIsGetCoverFromBaidu();
    const bool isBingEnabled = pSetting->getIsGetCoverFromBing();
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() {
        QByteArray data;
        if (isQQEnabled) {
            data = downloadQQMusicCover(key);
        }
        if (data.isEmpty() && isNetEaseEnabled) {
            data = downloadNetEaseCover(key);
        }
        if (data.isEmpty() && isBaiduEnabled) {
            data = downloadBaiduCover(key);
        }
        if (data.isEmpty() && isBingEnabled) {
            data = downloadBingCover(key);
        }
        showResourcePreview(pOnLine, url, data, true);
    });
}

void OnLine::downArtistCover(const QString &key, const QString &url) {
    const Setting *pSetting = &Setting::getInstance();
    if (!pSetting->getIsOnLine()) {
        return;
    }
    const bool isBaiduEnabled = pSetting->getIsGetCoverFromBaidu();
    const bool isBingEnabled = pSetting->getIsGetCoverFromBing();
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() {
        QByteArray data;
        if (isBaiduEnabled) {
            data = downloadBaiduCover(key);
        }
        if (isBingEnabled) {
            if (data.isEmpty()) {
                data = downloadBingCover(key);
            }
        }
        showResourcePreview(pOnLine, url, data, true);
    });
}

void OnLine::downAlbumCover(const QString &key, const QString &url) {
    downArtistCover(key, url);
}

void OnLine::downMusicCoverNetEase(const QString &key, const QString &url) {
    if (!Setting::getInstance().getIsOnLine()) {
        return;
    }
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() { showResourcePreview(pOnLine, url, downloadNetEaseCover(key), true); });
}

void OnLine::downMusicCoverQQMusic(const QString &key, const QString &url) {
    if (!Setting::getInstance().getIsOnLine()) {
        return;
    }
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() { showResourcePreview(pOnLine, url, downloadQQMusicCover(key), true); });
}

void OnLine::downCoverBing(const QString &key, const QString &url) {
    if (!Setting::getInstance().getIsOnLine()) {
        return;
    }
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() { showResourcePreview(pOnLine, url, downloadBingCover(key), true); });
}

void OnLine::downCoverBaidu(const QString &key, const QString &url) {
    if (!Setting::getInstance().getIsOnLine()) {
        return;
    }
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() { showResourcePreview(pOnLine, url, downloadBaiduCover(key), true); });
}

void OnLine::writeCoverToFile(const QString &url, const QString &filePath) {
    if (!Setting::getInstance().getIsOnLine()) {
        return;
    }
    OnLine *pOnLine = &OnLine::getInstance();
    runAsync([=]() { showResourcePreview(pOnLine, filePath, fetchUrl(QUrl(url)), true); });
}

void OnLine::savePreviewResource(const QString &filePath, const QString &data, const bool isImage) {
    const QByteArray bytes = isImage ? QByteArray::fromBase64(data.toLatin1()) : data.toUtf8();
    saveFile(bytes, filePath);
}

void OnLine::downLrc(const QString &key, const QString &url, const int musicId) {
    const Setting *pSetting = &Setting::getInstance();
    if (!pSetting->getIsOnLine()) {
        emit lrcDowned(musicId);
        return;
    }

    const bool isQQEnabled = pSetting->getIsGetLrcFromQQMusic();
    const bool isNetEaseEnabled = pSetting->getIsGetLrcFromNetEase();
    OnLine *pOnLine = this;
    runAsync([=]() {
        if (!QFile::exists(url)) {
            QByteArray data;
            if (isQQEnabled) {
                data = downloadQQMusicLyrics(key);
            }
            if (data.isEmpty() && isNetEaseEnabled) {
                data = downloadNetEaseLyrics(key);
            }
            showResourcePreview(pOnLine, url, data, false);
        }
        QMetaObject::invokeMethod(pOnLine, [pOnLine, musicId]() { emit pOnLine->lrcDowned(musicId); },
                                  Qt::QueuedConnection);
    });
}

void OnLine::downLrcFromNetEase(const QString &key, const QString &url, const int musicId) {
    if (!Setting::getInstance().getIsOnLine()) {
        if (musicId >= 0) {
            emit lrcDowned(musicId);
        }
        return;
    }
    OnLine *pOnLine = this;
    runAsync([=]() {
        showResourcePreview(pOnLine, url, downloadNetEaseLyrics(key), false);
        if (musicId >= 0) {
            QMetaObject::invokeMethod(pOnLine, [pOnLine, musicId]() { emit pOnLine->lrcDowned(musicId); },
                                      Qt::QueuedConnection);
        }
    });
}

void OnLine::downLrcFromQQMusic(const QString &key, const QString &url, const int musicId) {
    if (!Setting::getInstance().getIsOnLine()) {
        if (musicId >= 0) {
            emit lrcDowned(musicId);
        }
        return;
    }
    OnLine *pOnLine = this;
    runAsync([=]() {
        showResourcePreview(pOnLine, url, downloadQQMusicLyrics(key), false);
        if (musicId >= 0) {
            QMetaObject::invokeMethod(pOnLine, [pOnLine, musicId]() { emit pOnLine->lrcDowned(musicId); },
                                      Qt::QueuedConnection);
        }
    });
}

void OnLine::downMediaFromNetEase(const QString &search, const QString &musicUrl) {
    Q_UNUSED(search);
    Q_UNUSED(musicUrl);
}

void OnLine::downMediaFromQQMusic(const QString &search, const QString &musicUrl) {
    Q_UNUSED(search);
    Q_UNUSED(musicUrl);
}
