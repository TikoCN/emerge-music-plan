#include "mediaplayer.h"
#include "extralibrary.h"
#include "setting.h"
#include "hosttime.h"
#include "popupdata.h"
#include <QRandomGenerator>
#include <QAudioDevice>
#include <QPixmapCache>

MediaPlayer::MediaPlayer()
{
    loopType = 0;
    playingMusic = 0;
    playingCore = new Music;
    playingLrc = new LrcData;
    playingLrc->text = tr("Ciallo～(∠・ω< )⌒★");
    playedLrc = new LrcData;
    playedLrc->text = tr("~");

    player = new QMediaPlayer;//播放设备
    audioOutput = new QAudioOutput;//音频输出
    bufferOutput = new QAudioBufferOutput;//缓冲区输出

    player->setAudioOutput(audioOutput);
    player->setAudioBufferOutput(bufferOutput);

    connect(bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &MediaPlayer::buildFrequencySpectrum);

    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 time){
        updateAudioOutPut();
        selectPlayLrc(time);
    });
    //自动播放

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus staus){
        if(staus == QMediaPlayer::EndOfMedia)
        {
            playNext(1);
        }
    });
}

/*
 * 获得音乐核心
 */
void MediaPlayer::getMusicCoreAndCover(QList<Music*> musicList, QStringList musicKeyList)
{
    QStringList dirs;//本地文件列表
    QList<QList<Music *>> tableMusic;
    //生成播放列表和插入音乐
    for(int i=0; i< musicList.size(); i++){
        musicList[i]->coreId = coreList.size();
        coreList.append(musicList[i]);

        QString dir = musicList[i]->getParentDir();
        int aimTableId = dirs.indexOf(dir);
        if(aimTableId == -1){
            dirs.append(dir);
            aimTableId = dirs.size() - 1;
            QList<Music *> musics;
            tableMusic.append(musics);
        }

        tableMusic[aimTableId].append(musicList[i]);
    }
    for(int i=0; i<dirs.size(); i++){
        addTable(dirs[i], true);
        tableList[i]->insertMusic(tableMusic[i]);
    }

    buildNoDirTable(musicKeyList);

    //清空数据
    dirs.clear();
    tableMusic.clear();

    Setting *seit = Setting::getInstance();
    seit->musicKeyList.clear();
    delete seit->data;
    delete seit->coreJson;

    HostTime *host = HostTime::getInstance();
    host->musicFileList.clear();
    host->coreList.clear();
    host->musicKeyList.clear();
}

void MediaPlayer::buildNoDirTable(QStringList musicKeyList)
{
    //清空数据
    Setting *seit = Setting::getInstance();

    QJsonObject tableJson = seit->data->value("table").toObject();
    if(tableJson.isEmpty()){
        return;
    }
    int max = tableJson.value("size").toInt();//获得列表数

    for(int i=0; i<max; i++){
        QJsonObject table = tableJson.value(QString::number(i)).toObject();//获得列表数据单元
        QString name = table.value("name").toString();
        int aimTable = -1;

        if(table.value("isDir").toBool()){//处理本地列表
            QString url = table.value("url").toString();

            for(int j=0; j<tableList.size(); j++){
                if(tableList[j]->url == url){
                    aimTable = j;
                }
            }
        }
        else{//处理自建列表
            addTable(name);
            aimTable = tableList.size()-1;
            QStringList tableMusic = table.value("music").toString().split("||");
            int musicNumber = table.value("musicNumber").toInt();

            for(int j=0; j<musicNumber; j++){
                int musicId = tableMusic[j].toInt();
                if(musicId >= seit->musicKeyList.size()){
                    continue;
                }
                QString musicKey = seit->musicKeyList[musicId];
                int aim = musicKeyList.indexOf(musicKey);
                tableList[aimTable]->insertMusic(coreList[aim]);
            }
        }

        if(aimTable != -1){
            tableList[aimTable]->key = table.value("key").toInt();
            tableList[aimTable]->forward = table.value("forward").toBool();
            tableList[aimTable]->buildShowMusics();//排序
        }
    }
}

/*
 * 新建播放列表
 */
void MediaPlayer::addTable(QString tableName, bool isDir)
{
    PopupData *popup = PopupData::getInstance();
    if(tableName == ""){
        popup->message(tr("列表名不能为空"));
        return;
    }
    //判断该列表是否已经存在
    if(!isDir){
        for(int i=0; i<tableList.size(); i++){
            if(tableList[i]->name == tableName && !tableList[i]->isDir){
                popup->message(tr("该自建立列表已存在"));
                return;
            }
        }
    }

    Table* table = new Table;
    table->name = tableName;
    table->tableId = tableList.size();
    table->isDir = isDir;
    tableList.append(table);

    if(isDir){
        emit cppAddDirTable(table->tableId);
    }
    else{
        emit cppAddUserTable(table->tableId);
    }
}

/*
 * 将歌曲移动到
*/
void MediaPlayer::tableMoveMusic(int orgTableId, int musicId, int aimTalbeId)
{
    Music *core = tableList[orgTableId]->musics[musicId];
    tableList[orgTableId]->removeMusic(musicId);
    tableList[aimTalbeId]->insertMusic(core);
}

/*
 * 将歌词添加到播放下一首
*/
void MediaPlayer::playingInsertMusic(int coreId)
{
    Music *core = coreList[coreId];
    musicList.insert(playingMusic, core);

    emit cppPlayingInsertMusic(playingMusic);
}

/*
 * 将歌曲插入播放列表
*/
void MediaPlayer::musicInsertPlayingTable(int coreId)
{
    Music *core = coreList[coreId];

    musicList.append(core);

    emit cppMusicInsertPlayingTable(coreId);
}

void MediaPlayer::updateAudioOutPut()
{
    QAudioOutput* nowOut = new QAudioOutput;
    if(audioOutput->device().id() != nowOut->device().id())
    {
        audioOutput->setDevice(nowOut->device());


        player->setAudioOutput(audioOutput);
    }
    delete nowOut;
}

/*
 * 播放音乐
 */
void MediaPlayer::playMusic(int table, int music)
{
    if(table != -1){
        musicList.clear();
        musicList.append(tableList[table]->showMusics);

        emit cppBuildPlayingTable();
    }

    playingMusic = music;
    playingCore = musicList[music];
    musicList[music]->setPlayNumber(musicList[music]->playNumber+1);
    player->setSource(musicList[music]->url);
    player->play();

    loadLrcList(musicList[music]->getLrcUrl(), musicList[music]->endTime);
}

/*
 * 加载歌词
 */
void MediaPlayer::loadLrcList(QString lrcUrl, qint64 endTime)
{
    lrcList.clear();

    QFile file(lrcUrl);
    if(file.open(QIODevice::ReadOnly| QIODevice::Text)){
        QTextStream in(&file);
        QString line;
        do{
            line = in.readLine();//读取歌词
            //判断为有效行
            if(line.contains("]")){
                //插入歌词行
                LrcData *lrc = new LrcData;
                lrc->id = lrcList.size();
                lrcList.append(lrc);

                QString text = line.split("]")[1];
                QString date = line.split("[")[1].split("]")[0];

                if(line.contains(":") && line.contains("."))
                {
                    lrc->startTime = (date.split(":")[0].toInt() * 60
                                      + date.split(":")[1].split(".")[0].toInt()) *1000
                                     + date.split(":")[1].split(".")[1].toInt();
                }
                if(text == "")
                {
                    text = "~";
                }
                lrc->text = text.replace("/", "\n");
            }
        }
        while(!in.atEnd());
    }
    file.close();

    if(lrcList.size() > 1){
        for(int i=0; i<lrcList.size()-1;i++){
            lrcList[i]->endTime = lrcList[i+1]->startTime;
        }
        lrcList.last()->endTime = endTime;
    }
    else{
        LrcData *lrc = new LrcData;
        lrcList.append(lrc);
        lrc->text = tr("暂无歌词");
        lrc->startTime = 0;
        lrc->endTime = endTime;
    }

    playingLrc->id = 0;
    playingLrc->setText(lrcList[0]->text);
    if(lrcList.size() > 2){
        playedLrc->setText(lrcList[1]->text);
    }else{
        playedLrc->setText("~");
    }

    emit cppLrcLoaded(lrcList.size());
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    int play = playingLrc->id;

    //向下寻找
    while(play < lrcList.size()-1 && time > lrcList[play]->endTime){
        lrcList[play]->setPos(0);
        lrcList[play]->setIsPlay(false);
        play++;
    }

    //向前寻找
    while(play > 0 && lrcList[play]->startTime > time){
        lrcList[play]->setPos(0);
        lrcList[play]->setIsPlay(false);
        play--;
    }

    if(play != playingLrc->id){
        lrcList[play]->setIsPlay(true);
        playingLrc->setText(lrcList[play]->text);
        playingLrc->id = play;

        if(play < lrcList.size()-1){
            playedLrc->setText(lrcList[play + 1]->text);
        }
        else{
            playedLrc->setText(tr("歌曲已结束"));
        }

        emit cppPlayingLrc();
    }

    double pos = double(time - lrcList[play]->startTime) / double(lrcList[play]->endTime - lrcList[play]->startTime);
    lrcList[play]->setPos(pos);
    playingLrc->setPos(pos);
}

void MediaPlayer::turnToLrc(int lrcId)
{
    if(lrcId >=0 && lrcId < lrcList.size()){
        player->setPosition(lrcList[lrcId]->startTime);
    }
}

/*
 * 下一目标
 */
void MediaPlayer::playNext(int forward)
{
    int max = musicList.size();
    int aim = -1;

    if(max == 0){
        PopupData *popup = PopupData::getInstance();
        popup->message(tr("播放列表为空，请先播放音乐"));
        return;
    }
    switch (loopType) {
    case 0:
        aim = playingMusic + forward;
        break;
    case 1:
        aim = QRandomGenerator::global()->bounded(max);
        break;
    default:
        aim = playingMusic;
        break;
    }

    if(forward == 1 && aim >= max){
        aim = 0;
    }
    else if(aim < 0){
        aim = max - 1;
    }

    playMusic(-1, aim);
}

QString MediaPlayer::getTimeString()
{
    QDateTime time;
    time.setMSecsSinceEpoch(player->position());
    return time.toString("mm:ss.zzz");
}

void MediaPlayer::buildFrequencySpectrum(QAudioBuffer buffer)
{
    //得到所有音频样本
    const qint16* samples = buffer.constData<qint16>();
    int all = buffer.frameCount();//采样单元
    int sample = buffer.sampleCount();//总频道数

    if(all != 0)
    {
        int alone = sample/all;
        QVector<double>data(all);

        for (int i = 0; i < all; i++)
        {
            data[i] = 0.0;
            for(int j=0; j<alone && i*alone+j<sample; j++)
            {
                data[i] += samples[i*alone+j]/alone;
            }
        }
        //计算傅里叶变换
        ExtraLibrary ex;
        data = ex.useFftw3(data, all);

        //归一化
        for(int i=0; i<data.size()/2; i++)
        {
            if(data[i] > playingCore->maxHeight)
            {
                playingCore->maxHeight = data[i];
            }
            if(data[i] < playingCore->minHeight)
            {
                playingCore->minHeight = data[i];
            }
        }
        for(int i=0; i<data.size()/2; i++)
        {
            data[i] = (data[i]- playingCore->minHeight)/(playingCore->maxHeight - playingCore->minHeight);
            data[i] = data[i]<0?0:data[i];
        }

        QList<double> sampleList = data.mid(0, all/2);
        if(allSamples.size() <= 0){
            allSamples.fill(0.0, sampleList.size());
        }

        //时间平滑函数
        const double smoothConstantDown = 0.08;
        const double smoothConstantUp = 0.8;
        for(int i=0; i<sampleList.size() && i<allSamples.size(); i++){
            if(std::isfinite(sampleList[i])){//判断是不是有理数
                if(sampleList[i] > allSamples[i]){
                    allSamples[i] = smoothConstantUp * sampleList[i] + (1-smoothConstantUp) * allSamples[i];
                }
                else{
                    allSamples[i] = smoothConstantDown * sampleList[i] + (1-smoothConstantDown) * allSamples[i];
                }
            }
        }
        emit cppDrawLine();
    }
}

QAudioOutput *MediaPlayer::getAudioOutput() const
{
    return audioOutput;
}

void MediaPlayer::setAudioOutput(QAudioOutput *newAudioOutput)
{
    if (audioOutput == newAudioOutput)
        return;
    audioOutput = newAudioOutput;
    emit audioOutputChanged();
}

QVector<double> MediaPlayer::getAllSamples() const
{
    return allSamples;
}

void MediaPlayer::setAllSamples(const QVector<double> &newAllSamples)
{
    if (allSamples == newAllSamples)
        return;
    allSamples = newAllSamples;
    emit allSamplesChanged();
}

QList<Music *> MediaPlayer::getCoreList() const
{
    return coreList;
}

void MediaPlayer::setCoreList(const QList<Music *> &newCoreList)
{
    if (coreList == newCoreList)
        return;
    coreList = newCoreList;
    emit coreListChanged();
}

QList<Music *> MediaPlayer::getMusicList() const
{
    return musicList;
}

void MediaPlayer::setMusicList(const QList<Music *> &newMusicList)
{
    if (musicList == newMusicList)
        return;
    musicList = newMusicList;
    emit musicListChanged();
}

QList<LrcData *> MediaPlayer::getLrcList() const
{
    return lrcList;
}

void MediaPlayer::setLrcList(const QList<LrcData *> &newLrcList)
{
    if (lrcList == newLrcList)
        return;
    lrcList = newLrcList;
    emit lrcListChanged();
}

Music *MediaPlayer::getPlayingCore() const
{
    return playingCore;
}

void MediaPlayer::setPlayingCore(Music *newPlayingCore)
{
    if (playingCore == newPlayingCore)
        return;
    playingCore = newPlayingCore;
    emit playingCoreChanged();
}

int MediaPlayer::getLoopType() const
{
    return loopType;
}

void MediaPlayer::setLoopType(int newLoopType)
{
    if (loopType == newLoopType)
        return;
    loopType = newLoopType;
    emit loopTypeChanged();
}

QMediaPlayer *MediaPlayer::getPlayer() const
{
    return player;
}

void MediaPlayer::setPlayer(QMediaPlayer *newPlayer)
{
    if (player == newPlayer)
        return;
    player = newPlayer;
    emit playerChanged();
}

QList<Table *> MediaPlayer::getTableList() const
{
    return tableList;
}

void MediaPlayer::setTableList(const QList<Table *> &newTableList)
{
    if (tableList == newTableList)
        return;
    tableList = newTableList;
    emit tableListChanged();
}

LrcData *MediaPlayer::getPlayingLrc() const
{
    return playingLrc;
}

void MediaPlayer::setPlayingLrc(LrcData *newPlayingLrc)
{
    if (playingLrc == newPlayingLrc)
        return;
    playingLrc = newPlayingLrc;
    emit playingLrcChanged();
}

LrcData *MediaPlayer::getPlayedLrc() const
{
    return playedLrc;
}

void MediaPlayer::setPlayedLrc(LrcData *newPlayedLrc)
{
    if (playedLrc == newPlayedLrc)
        return;
    playedLrc = newPlayedLrc;
    emit playedLrcChanged();
}
