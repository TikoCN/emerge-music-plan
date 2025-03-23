#include "taskcenter.h"
#include <QDir>
#include <QUrl>
#include "mediaplayer.h"
#include "setting.h"
#include "musiccore.h"
#include "base.h"

TaskCenter::TaskCenter()
{
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();

    MediaPlayer* play = MediaPlayer::getInstance();
    connect(this, &TaskCenter::initData, play, &MediaPlayer::clearData);
    connect(play, &MediaPlayer::finishClearData, this, &TaskCenter::loadMusicFile);

    Setting* seit = Setting::getInstance();//获得设置指针
    connect(seit, &Setting::loadMusics, this, &TaskCenter::buildTaskCenter);
}

TaskCenter::~TaskCenter()
{
    thread->quit();
    thread->wait();
    delete thread;
}

/*
 *生成加载单元列表
 */
void TaskCenter::buildTaskCenter()
{
    //创建信号量
    semaphore = new QSemaphore(1);

    Setting* seit = Setting::getInstance();//获得设置指针
    for(int i=0; i<seit->maxThreadNumber; i++){
        BuildMusic* cell = new BuildMusic;
        buildMusicList.append(cell);

        cell->connect(this, &TaskCenter::startMusicCore, cell, &BuildMusic::loadMusicCore);

        cell->connect(cell, &BuildMusic::loadedMusicCore, this, &TaskCenter::getMusicCoreList);

        cell->connect(cell, &BuildMusic::finishMusicCore, this, &TaskCenter::finishMusicCore);
    }

    for(int i=0; i<seit->maxThreadNumber; i++){
        BuildUserTable* cell = new BuildUserTable;
        buildUserTableList.append(cell);

        cell->connect(this, &TaskCenter::startUserTable, cell, &BuildUserTable::loadUserTable);

        cell->connect(cell, &BuildUserTable::loadedUserTable, this, &TaskCenter::getUserTableMusic);

        cell->connect(cell, &BuildUserTable::finishUserTable, this, &TaskCenter::finishUserTable);
    }

    emit initData();
}

/*
 *遍历文件夹得到所有子文件
 */
void TaskCenter::filterFileInfo(QStringList dirPath)
{
    for (int i = 0; i < dirPath.size(); ++i) {
        QDir dir(QUrl(dirPath[i]).toLocalFile());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();

        if (fileList.size() > 0) {
            musicFileList.append(fileList);
            // 创建对应的本地列表
            Table *table = new Table;
            table->tableId = tableList.size();
            tableList.append(table);
            table->name = dir.dirName();
            table->url = dir.path();
            table->isDir = true;
            table->moveToThread(MusicCore::getInstance()->thread());
        }

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }
}

void TaskCenter::filterFileInfo(QFileInfoList dirs)
{
    for (int i = 0; i < dirs.size(); ++i) {
        QDir dir(dirs[i].filePath());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();

        if (fileList.size() > 0) {
            musicFileList.append(fileList);
            // 创建对应的本地列表
            Table *table = new Table;
            table->tableId = tableList.size();
            tableList.append(table);
            table->name = dir.dirName();
            table->url = dir.path();
            table->isDir = true;
            table->moveToThread(MusicCore::getInstance()->thread());
        }

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }
}

/*
 *加载音乐文件资源
 */
void TaskCenter::loadMusicFile()
{
    Setting* seit = Setting::getInstance();//获得设置指针

    //得到所有音乐文件
    filterFileInfo(seit->sourceList);

    workPos = 0;//重置工作位置
    workNumber = buildMusicList.size();//设置工作单元数
    emit startMusicCore();
}

void TaskCenter::loadUserTable()
{
    MusicCore *core = MusicCore::getInstance();
    QJsonObject data = core->readJsonData();
    QJsonObject music = data.value("music").toObject();
    QJsonObject table = data.value("table").toObject();

    // 得到旧的音乐对照表
    musicNameList = music.value("music").toString().split("|");

    int size = table.value("size").toInt();
    for (int i = 0; i < size; ++i) {
        QJsonObject tableJson = table.value(QString::number(i)).toObject();

        // 读取本地列表数据, 并排序
        if (tableJson.value("isDir").toBool()) {
            for (int j = 0; j < tableList.size(); ++j) {
                if (tableList[j]->url == tableJson.value("url")) {
                    tableList[j]->sortMusic(tableJson.value("sort").toInt());
                    break;
                }
            }
        }
        else {
            // 创建列表
            Table *table = new Table;
            table->tableId = tableList.size();
            tableList.append(table);
            table->name = tableJson.value("name").toString();
            table->isDir = tableJson.value("isDir").toBool();
            table->sortMusic(tableJson.value("sort").toInt());
            table->moveToThread(core->thread());

            // 得到歌曲附录
            QStringList musicIdList = tableJson.value("music").toString().split("|");
            tableMusic.append(musicIdList);
        }
    }

    workNumber = buildUserTableList.size();
    workPos = 0;
    tableId = dirTableSize;
    emit startUserTable();
}

void TaskCenter::insertArtist(Music *music)
{
    for (int k = 0; k < music->artistList.size(); ++k) {
        QString artistName = music->artistList[k];

        bool isNoFind = true;
        for (int i = 0; i < artistLineList.size(); ++i) {
            QList<Artist *> artistList = artistLineList[i];

            if (artistList.first()->name[0] != artistName[0]) {
                continue;
            }

            // 遍历行
            for (int j = 0; j < artistList.size(); ++j) {
                // 遍历作者列表
                if (artistList[j]->name == artistName) {
                    isNoFind = false;
                    artistList[j]->musicList.append(music);
                    break;
                }
            }

            if (isNoFind) {
                isNoFind = false;
                Artist *artist = new Artist(artistName);
                artist->musicList.append(music);
                artist->id = artistLineList.size();
                artist->moveToThread(MusicCore::getInstance()->thread());

                artistLineList[i].append(artist);
            }
        }

        if (isNoFind) {
            Artist *artist = new Artist(artistName);
            artist->musicList.append(music);
            artist->id = artistLineList.size();
            artist->moveToThread(MusicCore::getInstance()->thread());
            QList<Artist *> artistList;

            artistList.append(artist);
            artistLineList.append(artistList);
        }
    }
}

void TaskCenter::insertAlumb(Music *music)
{
    bool isNoFind = true;
    for (int i = 0; i < alumbLineList.size(); ++i) {
        QList<Alumb *> alumbList = alumbLineList[i];

        if (alumbList.first()->name[0] != music->alumb[0]) {
            continue;
        }

        for (int j = 0; j < alumbList.size(); ++j) {
            if (alumbList[j]->name == music->alumb) {
                isNoFind = false;
                alumbList[j]->musicList.append(music);
                break;
            }
        }

        if (isNoFind) {
            isNoFind = false;
            Alumb *alumb = new Alumb(music->alumb);
            alumb->musicList.append(music);
            alumb->id = artistLineList.size();
            alumb->moveToThread(MusicCore::getInstance()->thread());

            alumbLineList[i].append(alumb);
        }
    }

    if (isNoFind) {
        Alumb *alumb = new Alumb(music->alumb);
        alumb->musicList.append(music);
        alumb->id = artistLineList.size();
        alumb->moveToThread(MusicCore::getInstance()->thread());
        QList<Alumb *> alumbList;

        alumbList.append(alumb);
        alumbLineList.append(alumbList);
    }
}

/*
 * 申请分配文件列表
 */
bool TaskCenter::getInfoList(QFileInfoList *list)
{
    semaphore->acquire();                      //请求读写
    //查看任务是否完成，已经任务队列是否为空
    if(workPos > musicFileList.size() - 1 || musicFileList.size() <= 0){
        semaphore->release();
        return false;
    }

    int length = 10;
    int start = workPos;
    if(workPos + length >= musicFileList.size()){
        length = musicFileList.size() - workPos;
    }
    *list = musicFileList.mid(start, length);
    workPos = start + length;
    semaphore->release();                      //释放读写

    return true;
}

bool TaskCenter::getUserTableTask(QStringList *musicNameList, int *aim)
{
    semaphore->acquire();                      //请求读写
    QStringList musicIdList;
    while (true) {
        // 任务结束
        if (
            tableMusic.size() <= 0 ||
            tableId == tableList.size() ||
            tableList.size() <= 0
            ) {
            semaphore->release();
            return false;
        }

        musicIdList = tableMusic[tableId - dirTableSize];

        //查看任务是否完成，已经任务队列是否为空
        if(workPos > musicIdList.size() - 1 || musicIdList.size() <= 0){
            tableId++;
        }
        else{
            break;
        }
    }

    int length = 30;
    int start = workPos;
    if(workPos + length >= musicIdList.size()){
        length = musicIdList.size() - workPos;
    }

    // 返回数据
    *musicNameList = musicIdList.mid(start, length);
    *aim = tableId;

    // 移动工作点
    workPos = start + length;
    semaphore->release();

    return true;
}

/*
 *获得加载好的音乐数据
 */
void TaskCenter::getMusicCoreList(QList<Music *> musicList, QList<QList<Music *>> tableMusicList)
{
    for (int i = 0; i < tableMusicList.size(); ++i) {
        tableList[i]->musics.append(tableMusicList[i]);
        tableList[i]->showMusics.append(tableMusicList[i]);
    }

    // 遍历获得的音乐列表
    for (int i = 0; i < musicList.size(); ++i) {
        Music *music = musicList[i];

        music->coreId = this->musicList.size();
        this->musicList.append(music);

        insertAlumb(music);
        insertArtist(music);
    }

    dirTableSize = tableList.size();
}

void TaskCenter::getUserTableMusic(QList<Music *> musicList, int tableId)
{
    if (tableId < 0 || tableId >= tableList.size()){
        return;
    }
    // 插入列表
    tableList[tableId]->insertMusic(musicList);
}

void TaskCenter::finishMusicCore()
{
    //计算当前工作单元数
    workNumber--;
    if(workNumber == 0){
        loadUserTable();
    }
}

void TaskCenter::finishUserTable()
{
    //计算当前工作单元数
    workNumber--;
    if(workNumber == 0){
        QString t = tr("加载音乐文件完成，加载了 ") + QString::number(musicList.size()) + tr(" 个音乐文件");
        emit Base::getInstance()->sendMessage(t, 0);

        std::sort(artistLineList.begin(), artistLineList.end(), [this](QList<Artist *> a, QList<Artist *> b){
            return a[0]->name[0] < b[0]->name[0];
        });

        std::sort(alumbLineList.begin(), alumbLineList.end(), [this](QList<Alumb *> a, QList<Alumb *> b){
            return a[0]->name[0] < b[0]->name[0];
        });

        emit musicsLoaded(musicList, tableList, artistLineList, alumbLineList);
    }
}

/*
 *删除数据
 */
void TaskCenter::clearData()
{
    this->musicList.clear();
    this->musicFileList.clear();
    this->musicNameList.clear();
    this->tableMusic.clear();
    this->alumbLineList.clear();
    this->artistLineList.clear();

    //从头删除到尾
    while (!buildMusicList.empty()) {
        delete buildMusicList.takeFirst();
    }

    //从头删除到尾
    while (!buildUserTableList.empty()) {
        delete buildUserTableList.takeFirst();
    }

    //删除信号量
    delete semaphore;
}
