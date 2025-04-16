#include "taskcenter.h"
#include <QDir>
#include <QUrl>
#include "mediaplayer.h"
#include "setting.h"
#include "musiccore.h"
#include "base.h"
#include "sqlite.h"

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
    chinesePinyin = Base::getInstance()->getChineseToPinyinJson();
    for (char i = 'A'; i <= 'Z'; ++i) {
        keyList.append(QString(i));
    }

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

        insertAlbum(music);
        insertArtist(music);
    }

    dirTableSize = tableList.size();
}

int TaskCenter::getEnglishId(QString name)
{
    for (int i = 0; i < chinesePinyin.size(); ++i) {
        if (chinesePinyin[i].indexOf(name[0]) != -1) {
            return i;
        }
    }
    return -1;
}

void TaskCenter::insertArtist(Music *music)
{
    for (int k = 0; k < music->artistList.size(); ++k) {
        QString artistName = music->artistList[k];

        bool isNoFind = true;
        for (int i = 0; i < artistList.size(); ++i) {
            if(artistList[i]->name == artistName) {
                artistList[i]->musicList.append(music);
                isNoFind = false;
                break;
            }
        }

        if (isNoFind) {
            Artist *artist = new Artist(artistName);
            artist->id = artistList.size();
            int aimLineKey = getEnglishId(artist->name);
            // 设置行关键字
            if (aimLineKey == -1) {
                artist->lineKey = artist->name[0];
            }
            else {
                artist->lineKey = keyList[aimLineKey];
            }

            artist->musicList.append(music);
            artistList.append(artist);
        }
    }
}

void TaskCenter::insertAlbum(Music *music)
{
    QString albumName = music->album;

    bool isNoFind = true;
    for (int i = 0; i < albumList.size(); ++i) {
        if(albumList[i]->name == albumName) {
            albumList[i]->musicList.append(music);
            music->albumId = albumList[i]->id;

            // 在专辑中添加歌手
            for (int j = 0; j < music->artistList.size(); ++j) {
                albumList[i]->artistSet.insert(music->artistList[j]);
            }
            isNoFind = false;
            break;
        }
    }

    if (isNoFind) {
        Album *album = new Album(albumName);
        album->id = albumList.size();
        int aimLineKey = getEnglishId(album->name);
        // 设置行关键字
        if (aimLineKey == -1) {
            album->lineKey = album->name[0];
        }
        else {
            album->lineKey = keyList[aimLineKey];
        }

        album->musicList.append(music);
        music->albumId = album->id;
        // 在专辑中添加歌手
        for (int j = 0; j < music->artistList.size(); ++j) {
            album->artistSet.insert(music->artistList[j]);
        }
        albumList.append(album);
    }
}

void TaskCenter::getUserTableMusic(QList<Music *> musicList, int tableId)
{
    if (tableId < 0 || tableId >= tableList.size()){
        return;
    }
    // 插入列表
    tableList[tableId]->appendMusic(musicList);
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
        Base::getInstance()->sendMessage(t, 0);

        std::sort(artistList.begin(), artistList.end(), [](Artist *a, Artist *b)->bool{
            return a->lineKey < b->lineKey;
        });

        std::sort(albumList.begin(), albumList.end(), [](Album *a, Album *b)->bool{
            return a->lineKey < b->lineKey;
        });

        writeDataSQL();
        emit musicsLoaded(musicList, tableList, artistList, albumList);
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
    this->albumList.clear();
    this->artistList.clear();
    this->chinesePinyin.clear();
    this->keyList.clear();

    //从头删除到尾
    while (!buildMusicList.empty()) {
        delete buildMusicList.takeLast();
    }

    //从头删除到尾
    while (!buildUserTableList.empty()) {
        delete buildUserTableList.takeLast();
    }

    //删除信号量
    delete semaphore;
}

void TaskCenter::writeDataSQL()
{
    SQLite *sql = SQLite::getInstance();
    bool flag;
    flag = sql->begin();
    if (!flag) qDebug()<< flag;

    flag = sql->updateAlbum(albumList);
    if (!flag) qDebug()<< flag;

    flag = sql->updateArtist(artistList);
    if (!flag) qDebug()<< flag;

    flag = sql->updateMusic(musicList);
    if (!flag) qDebug()<< flag;

    flag = sql->updateTable(tableList);
    if (!flag) qDebug()<< flag;

    flag = sql->updateTableMusic(tableList);
    if (!flag) qDebug()<< flag;

    flag = sql->updateArtistMusic(artistList);
    if (!flag) qDebug()<< flag;

    flag = sql->commit();
    if (!flag) qDebug()<< flag;
}
