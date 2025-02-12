#include "hosttime.h"
#include "extralibrary.h"
#include "setting.h"
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

TaskCell::TaskCell(){
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}
TaskCell::~TaskCell(){

}

/*
 *加载文件夹中的音乐
 */
void TaskCell::working(){
    ExtraLibrary extraLibrary;
    Setting *seit = Setting::getInstance();
    HostTime *host = HostTime::getInstance();
    QJsonObject *coreJson = seit->coreJson;
    QStringList *musicKeyList = &seit->musicKeyList;

    while(true){
        QFileInfoList fileList;
        //申请控件
        if(!host->getInfoList(&fileList)){
            break;
        }

        QList<Music *> cores;
        QStringList keys;

        for(int i=0; i<fileList.size(); i++){
            QString suffix = fileList[i].suffix();
            if(suffix != "mp3" && suffix != "flac"){
                continue;
            }

            //加载音乐数据
            Music *core = new Music;
            core->fromFileInfo(fileList[i]);
            extraLibrary.getMedia(core);
            cores.append(core);
            keys.append(core->getKey());

            int aim = musicKeyList->indexOf(core->getKey());
            if(aim != -1){
                core->fromJson(coreJson->value(QString::number(aim)).toObject());
            }

            //移动到主线程
            core->moveToThread(QCoreApplication::instance()->thread());
        }
        emit musicLoaded(cores, keys);
    }

    emit finishLoad(this);
}

HostTime::HostTime()
{
    buildTaskCellList();
    Setting* seit = Setting::getInstance();
    connect(seit, &Setting::musicCoresChange, this, &HostTime::loadMusicFile);
    semaphore = new QSemaphore(1);
}

/*
 *生成加载单元列表
 */
void HostTime::buildTaskCellList()
{
    Setting* seit = Setting::getInstance();//获得设置指针
    for(int i=0; i<seit->maxThreadNumber; i++){
        TaskCell* cell = new TaskCell;
        taskCellList.append(cell);

        connect(this, &HostTime::startWork, cell, &TaskCell::working);
        connect(cell, &TaskCell::musicLoaded, this, &HostTime::getMusicCoreList);
        connect(cell, &TaskCell::finishLoad, this, &HostTime::cellFinishWork);
    }
}

/*
 *加载音乐文件资源
 */
void HostTime::loadMusicFile(QStringList dirList)
{
    //得到所有音乐文件
    for(int i=0; i<dirList.size(); i++){
        QUrl url(dirList[i]);
        musicFileList.append(getMusicUrl(url.toLocalFile()));
    }

    workNumber = taskCellList.size();
    emit startWork();
}

/*
 *遍历文件夹得到所有子文件
 */
QFileInfoList HostTime::getMusicUrl(QString dirPath)
{
    QDir dir(dirPath);
    QFileInfoList allFileList;
    QFileInfoList childDirList;

    //得到子文件夹
    dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
    childDirList = dir.entryInfoList();

    //得到子文件
    dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
    allFileList = dir.entryInfoList();
    for(int i=0; i<childDirList.size(); i++){
        allFileList.append(getMusicUrl(childDirList[i].filePath()));
    }

    return allFileList;
}

bool HostTime::getInfoList(QFileInfoList *list)
{
    semaphore->acquire();
    if(workPos >= musicFileList.size() - 1){
        semaphore->release();
        return false;
    }

    int length = 30;
    int start = workPos;
    if(workPos + length >= musicFileList.size()){
        length = musicFileList.size() - workPos - 1;
    }
    workPos = start + length;
    *list = musicFileList.mid(workPos, length);
    semaphore->release();

    return true;
}

/*
 *获得加载好的音乐数据
 */
void HostTime::getMusicCoreList(QList<Music *> coreList, QStringList musicKeyList)
{
    this->musicKeyList.append(musicKeyList);
    this->coreList.append(coreList);
}

void HostTime::cellFinishWork(TaskCell *cell)
{
    workNumber--;
    if(workNumber == 0){
        emit musicsLoaded(coreList, musicKeyList);
    }
}

void HostTime::onLineGetCoverFromNetEase(QString title, QString url)
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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"blurPicUrl\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        onLineCoverToFile(matchList[1], url);
}

void HostTime::onLineGetCoverFromQQMusic(QString key, QString url)
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
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    htmlData = reply->readAll();
    regularExpression.setPattern("\"albummid\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;;

    request.setUrl(QUrl("http://y.gtimg.cn/music/photo_new/T002R180x180M000"
                        +matchList[1]
                        +".jpg"));
    reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    htmlData = reply->readAll();
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        onLineCoverToFile(matchList[1], url);
}

void HostTime::onLineGetCoverFromBing(QString key, QString url)
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
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("src=\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        onLineCoverToFile(matchList[1], url);
}

void HostTime::onLineGetCoverFromBaidu(QString key, QString url)
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
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //解析
    htmlData = reply->readAll();
    regularExpression.setPattern("\"thumbURL\":\"([^\"]*),");
    matchList = regularExpression.match(htmlData).capturedTexts();

    if(matchList.size() >= 2)
        onLineCoverToFile(matchList[1], url);
}

void HostTime::onLineCoverToFile(QString url, QString fileUrl)
{
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QEventLoop loop;

    //得到实际图片数据
    request.setUrl(QUrl(url));
    reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //写入文件
    QFile file(fileUrl);
    if(file.open(QIODevice::WriteOnly)){
        file.write(reply->readAll());
    }
    file.close();
}

void HostTime::onLineGetLrcFromNetEase(QString key, QString url)
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
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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

void HostTime::onLineGetLrcFromQQMusic(QString key, QString url)
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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();//开始阻塞

    htmlData = reply->readAll();
    regularExpression.setPattern("\"lyric\":\"([^\"]*)\"");
    matchList = regularExpression.match(htmlData).capturedTexts();
    if(matchList.size() < 2)//判断解析是否成功
        return;

    QFile file(QDir::currentPath()+"/new.lrc");
    if(file.open(QIODevice::WriteOnly)){
        file.write(matchList[1].replace("\\n", "\n").toUtf8());
    }
    file.close();
}
