#include "online.h"
#include "setting.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <QThread>

OnLine::OnLine()
{
    QThread* thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}

void OnLine::downMusicCover(QString key, QString url)
{
    Setting *seit = Setting::getInstance();
    if(!seit->isOnLine){ //未开启网络模块，退出
        return;
    }

    if(seit->isGetCoverFromQQMusic && !QFile::exists(url)){
        downMusicCoverQQMusic(key, url);
    }

    if(seit->isGetCoverFromNetEase && !QFile::exists(url)){
        downMusicCoverNetEase(key, url);
    }

    if(seit->isGetCoverFromBaidu && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->isGetCoverFromBing && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downArtistCover(QString key, QString url)
{
    Setting *seit = Setting::getInstance();
    if(!seit->isOnLine){ //未开启网络模块，退出
        return;
    }

    if(seit->isGetCoverFromBaidu && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->isGetCoverFromBing && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downAlbumCover(QString key, QString url)
{
    Setting *seit = Setting::getInstance();
    if(!seit->isOnLine){ //未开启网络模块，退出
        return;
    }

    if(seit->isGetCoverFromBaidu && !QFile::exists(url)){
        downCoverBaidu(key, url);
    }

    if(seit->isGetCoverFromBing && !QFile::exists(url)){
        downCoverBing(key, url);
    }
}

void OnLine::downMusicCoverNetEase(QString title, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QEventLoop loop;
    QStringList matchList;//正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    //得到音乐id
    request.setUrl(QUrl("https://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s="
                        +title
                        +"&type=1&offset=0&total=true&limit=1"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"id\":([^,]*),");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //得到图片地址
    request.setUrl(QUrl("http://music.163.com/api/song/detail/?id="
                        +matchList[1]
                        +"&ids=%5B"
                        +matchList[1]
                        +"%5D"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"blurPicUrl\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downMusicCoverQQMusic(QString key, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QStringList matchList;//正则表达式结果
    QEventLoop loop;
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="
                        +key
                        +"&format=json"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    htmlData = reply->readAll();
    regularExpression.setPattern("\"albummid\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    request.setUrl(QUrl("http://y.gtimg.cn/music/photo_new/T002R180x180M000"
                        +matchList[1]
                        +".jpg"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    htmlData = reply->readAll();
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downCoverBing(QString key, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QEventLoop loop;
    QStringList matchList;//正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://www.bing.com/images/async?q="
                        +key
                        +"&first=1&count=10&mmasync=1"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("src=\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::downCoverBaidu(QString key, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QEventLoop loop;
    QStringList matchList;//正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://image.baidu.com/search/acjson?tn=resultjson_com&logid=8212365409067552127&ipn=rj&ct=201326592&is=&fp=result&fr=&word="
                        +key+
                        "&queryWord="
                        +key+
                        "&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=&z=&ic=&hd=&latest=©right=&s=&se=&tab=&width=&height=&face=&istype=&qc=&nc=1&expermode=&nojc=&isAsync=&pn=1&rn=10&gsm=5a&1668600962847="));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"thumbURL\":\"([^\"]*),");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        writeCoverToFile(matchList[1], url);
}

void OnLine::writeCoverToFile(QString url, QString fileUrl)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QEventLoop loop;

    //得到实际图片数据
    request.setUrl(QUrl(url));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //写入文件
    QFile file(fileUrl);
    if(file.open(QIODevice::WriteOnly)){
        file.write(reply->readAll());
    }
    file.close();
}

void OnLine::downLrc(QString key, QString url)
{
    Setting *seit = Setting::getInstance();
    if(!seit->isOnLine){ //未开启网络模块，退出
        emit lrcDowned();
        return;
    }

    if(seit->isGetLrcFromQQMusic && !QFile::exists(url)){
        downLrcFromQQMusic(key, url);
    }

    if(seit->isGetCoverFromNetEase && !QFile::exists(url)){
        downLrcFromNetEase(key, url);
    }
    emit lrcDowned();
}

void OnLine::downLrcFromNetEase(QString key, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QEventLoop loop;
    QStringList matchList;//正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    //得到音乐id
    request.setUrl(QUrl("https://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s="
                        +key+
                        +"&type=1&offset=0&total=true&limit=1"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"id\":([^,]*),");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    //得到图片地址
    request.setUrl(QUrl("http://music.163.com/api/song/lyric?id="
                        + matchList[1]
                        + "&lv=1&kv=1&tv=-1"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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

void OnLine::downLrcFromQQMusic(QString key, QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString htmlData;
    QEventLoop loop;
    QStringList matchList;//正则表达式结果
    QRegularExpression regularExpression;//正则表达式

    request.setUrl(QUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="
                        +key
                        +"&format=json"));
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();//开始阻塞

    htmlData = reply->readAll();
    regularExpression.setPattern("\"songmid\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    request.setUrl(QUrl("https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid="
                        +matchList[1]
                        +"&format=json&nobase64=1"));
    request.setRawHeader("Referer" , "https//y.qq.com/portal/player.html");
    reply = manager.get(request);
    loop.connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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

void OnLine::downMediaFromNetEase(QString search, QString musicUrl)
{

}

void OnLine::downMediaFromQQMusic(QString search, QString musicUrl)
{

}
