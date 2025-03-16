#include "mediaplayer.h"
#include "extralibrary.h"
#include "setting.h"
#include "hosttime.h"
#include "online.h"
#include <QRandomGenerator>
#include <QAudioDevice>
#include <QPixmapCache>

void MediaPlayer::clearData()
{
    while (!tableList.empty()) {
        delete tableList.takeFirst();
    }

    while (!coreList.empty()) {
        delete coreList.takeFirst();
    }

    while (!musicList.empty()) {
        delete musicList.takeFirst();
    }

    while (!lrcList.empty()) {
        delete lrcList.takeFirst();
    }

    allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
}

/*
 * 获得音乐核心
 */
void MediaPlayer::getMusicCore(QList<Music*> musicList)
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

    //生成本地列表
    for(int i=0; i<dirs.size(); i++){
        appendTable(dirs[i], true);
        tableList[i]->insertMusic(tableMusic[i]);
    }

    //清空数据
    HostTime *host = HostTime::getInstance();

    host->clearData();

    //初始化部分数据
    playingCore->title = tr("歌曲标题");
    playingCore->artist = tr("歌手");
    allSamples.fill(0, 1024);

    LrcData *lrc = new LrcData;
    lrcList.append(lrc);
}

/*
 * 新建播放列表
 */
void MediaPlayer::appendTable(QString tableName, bool isDir)
{
    //判断该列表是否已经存在
    if(!isDir){
        for(int i=0; i<tableList.size(); i++){
            if(tableList[i]->name == tableName && !tableList[i]->isDir){
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
        //设置本地列表参数
        table->url = tableName;
        table->name = tableName.split("/").last();
    }

    emit addTable(table->tableId);
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
    player->setSource(musicList[music]->url);
    player->play();

    lrcList.clear();//清空遗留歌词
    emit downLrc(playingCore->getBaseName(), playingCore->getLrcUrl());//加载新歌词
}

/*
 * 加载歌词
 */
void MediaPlayer::loadLrcList()
{
    lrcList = playingCore->getLyricsData();
    emit cppLrcLoaded(lrcList.size());
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    if(lrcList.size() == 0){
        return;
    }

    int play = playingLrc->id;

    if(playingLrc->startTime <= time && playingLrc->endTime >= time){
        emit lrcList[play]->update();
        emit playingLrc->update();
    }
    else{
        for(int i=0; i<lrcList.size(); i++){
            if(lrcList[i]->startTime <= time && lrcList[i]->endTime >= time){
                playingLrc->id = i;
                emit playingLrcLineChange();
                emit lrcList[i]->update();
                break;
            }
        }
    }

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

    if(all != 0){
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
        double maxHeight = 0.0, minHeight = 0.0;
        for(int i=0; i<data.size()/2; i++)
        {
            if(data[i] > maxHeight)
            {
                maxHeight = data[i];
            }
            if(data[i] < minHeight)
            {
                minHeight = data[i];
            }
        }
        for(int i=0; i<data.size()/2; i++)
        {
            data[i] = (data[i]- minHeight)/(maxHeight - minHeight);
            data[i] = data[i]<0?0:data[i];
        }

        QList<double> sampleList = data.mid(0, all/2);
        if(allSamples.size() != sampleList.size()){
            allSamples.clear();
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

        int aim = 120;
        int cell = allSamples.size() / aim;
        QVector<double> outSamples;//处理之后的音乐样本
        outSamples.fill(0, aim);
        cell = cell <= 0 ? 1 : cell;
        for(int i=0; i<aim && i * cell < allSamples.size(); i++){
            double max = allSamples[i * cell];

            for(int j=1; j<cell && i * cell + j < allSamples.size(); j++){
                if(max < allSamples[i * cell + j]){
                    max = allSamples[i * cell + j];
                }
            }
            outSamples[i] = max;
        }
        emit cppDrawLine(outSamples);
    }
}

MediaPlayer::MediaPlayer()
{
    loopType = 0;
    playingMusic = 0;
    playingCore = new Music;
    playingLrc = new LrcData;
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

    //关联网络模块
    OnLine *onLine = OnLine::getInstance();
    connect(this, &MediaPlayer::downLrc, onLine, &OnLine::downLrc);
    connect(onLine, &OnLine::lrcDowned, this, &MediaPlayer::loadLrcList);
}

QAudioOutput *MediaPlayer::getAudioOutput() const
{
    return audioOutput;
}

QVector<double> MediaPlayer::getAllSamples() const
{
    return allSamples;
}

QList<Music *> MediaPlayer::getCoreList() const
{
    return coreList;
}

QList<Music *> MediaPlayer::getMusicList() const
{
    return musicList;
}

QList<LrcData *> MediaPlayer::getLrcList() const
{
    return lrcList;
}

Music *MediaPlayer::getPlayingCore() const
{
    return playingCore;
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

QList<Table *> MediaPlayer::getTableList() const
{
    return tableList;
}

LrcData *MediaPlayer::getPlayingLrc() const
{
    return playingLrc;
}

