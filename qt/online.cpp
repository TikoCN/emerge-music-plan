#include "online.h"
#include "setting.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QRegularExpression>
#include <QDir>
#include <QThread>

OnLine::OnLine()
{
    auto* thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}

void OnLine::downMusicCover(const QString& key, const QString& url)
{
    const Setting *seit = Setting::getInstance();
    if(!seit->getIsOnLine()){ //未开启网络模块，退出
        return;
    }

    if(seit->getIsGetCoverFromQQMusic() && !QFile::exists(url)){
        downMusicCoverQQMusic(key, url);
    }

    if(seit->getIsGetCoverFromNetEase() && !QFile::exists(url)){
        downMusicCoverNetEase(key, url);
    }

    if(seit->getIsGetCoverFromBaidu() && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->getIsGetCoverFromBing() && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downArtistCover(const QString& key, const QString& url)
{
    const Setting *seit = Setting::getInstance();
    if(!seit->getIsOnLine()){ //未开启网络模块，退出
        return;
    }

    if(seit->getIsGetCoverFromBaidu() && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->getIsGetCoverFromBing() && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downAlbumCover(const QString& key, const QString& url)
{
    const Setting *seit = Setting::getInstance();
    if(!seit->getIsOnLine()){ //未开启网络模块，退出
        return;
    }

    if(seit->getIsGetCoverFromBaidu() && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->getIsGetCoverFromBing() && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downMusicCoverNetEase(const QString& title, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    //得到音乐id
    request.setUrl(QUrl("https://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s="
                        +title
                        +"&type=1&offset=0&total=true&limit=1"));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    QString htmlData = reply->readAll();
    regularExpression.setPattern("\"id\":([^,]*),");
    QStringList matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //得到图片地址
    request.setUrl(QUrl("http://music.163.com/api/song/detail/?id="
                        +matchList[1]
                        +"&ids=%5B"
                        +matchList[1]
                        +"%5D"));
    reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"blurPicUrl\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downMusicCoverQQMusic(const QString& key, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    //正则表达式结果
    QEventLoop loop;
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="
                        +key
                        +"&format=json"));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString htmlData = reply->readAll();
    regularExpression.setPattern("\"albummid\":\"([^\"]*)\"");
    QStringList matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    request.setUrl(QUrl("http://y.gtimg.cn/music/photo_new/T002R180x180M000"
                        +matchList[1]
                        +".jpg"));
    reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    htmlData = reply->readAll();
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downCoverBing(const QString &key, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://www.bing.com/images/async?q="
                        +key
                        +"&first=1&count=10&mmasync=1"));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    const QString htmlData = reply->readAll();
    regularExpression.setPattern("src=\"([^\"]*)\"");

    if(QStringList matchList = regularExpression.match(htmlData).capturedTexts(); matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downCoverBaidu(const QString& key, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://image.baidu.com/search/acjson?tn=resultjson_com&logid=8212365409067552127&ipn=rj&ct=201326592&is=&fp=result&fr=&word="
                        +key+
                        "&queryWord="
                        +key+
                        "&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=&z=&ic=&hd=&latest=©right=&s=&se=&tab=&width=&height=&face=&istype=&qc=&nc=1&expermode=&nojc=&isAsync=&pn=1&rn=10&gsm=5a&1668600962847="));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    const QString htmlData = reply->readAll();
    regularExpression.setPattern(R"("thumbURL":"([^"]*),)");

    if(QStringList matchList = regularExpression.match(htmlData).capturedTexts(); matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::writeCoverToFile(const QString& url, const QString& fileUrl)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;

    //得到实际图片数据
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //写入文件
    QFile file(fileUrl);
    if(file.open(QIODevice::WriteOnly)){
        file.write(reply->readAll());
    }
    file.close();
}

void OnLine::downLrc(const QString& key, const QString &url, const int musicId)
{
    const Setting *seit = Setting::getInstance();
    if(!seit->getIsOnLine()){ //未开启网络模块，退出
        emit lrcDowned(musicId);
        return;
    }

    if(seit->getIsGetLrcFromQQMusic() && !QFile::exists(url)){
        downLrcFromQQMusic(key, url);
    }

    if(seit->getIsGetCoverFromNetEase() && !QFile::exists(url)){
        downLrcFromNetEase(key, url);
    }
    emit lrcDowned(musicId);
}

void OnLine::downLrcFromNetEase(const QString& key, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    //得到音乐id
    request.setUrl(QUrl("https://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s="
                        +key+
                        +"&type=1&offset=0&total=true&limit=1"));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    QString htmlData = reply->readAll();
    regularExpression.setPattern("\"id\":([^,]*),");
    QStringList matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //得到图片地址
    request.setUrl(QUrl("http://music.163.com/api/song/lyric?id="
                        + matchList[1]
                        + "&lv=1&kv=1&tv=-1"));
    reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"lyric\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //写入文件
    QFile file(url);
    if(file.open(QIODevice::WriteOnly| QIODevice::Text)){
        QTextStream out(&file);
        out<<matchList[1].replace("\\n", "\n");
    }
    file.close();
}

void OnLine::downLrcFromQQMusic(const QString& key, const QString &url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QEventLoop loop;
    //正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="
                        +key
                        +"&format=json"));
    QNetworkReply *reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();//开始阻塞

    QString htmlData = reply->readAll();
    regularExpression.setPattern("\"songmid\":\"([^\"]*)\"");
    QStringList matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    request.setUrl(QUrl("https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid="
                        +matchList[1]
                        +"&format=json&nobase64=1"));
    request.setRawHeader("Referer" , "https//y.qq.com/portal/player.html");
    reply = manager.get(request);
    QEventLoop::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();//开始阻塞

    htmlData = reply->readAll();
    regularExpression.setPattern("\"lyric\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //写入文件
    QFile file(url);
    if(file.open(QIODevice::WriteOnly| QIODevice::Text)){
        QTextStream out(&file);
        out<<matchList[1].replace("\\n", "\n");
    }
    file.close();
}

void OnLine::downMediaFromNetEase(const QString& search, const QString& musicUrl)
{

}

void OnLine::downMediaFromQQMusic(const QString& search, const QString& musicUrl)
{

}
