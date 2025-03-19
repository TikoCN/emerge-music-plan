#include "mediaplayer.h"
#include "extralibrary.h"
#include <QRandomGenerator>
#include <QAudioDevice>
#include <QPixmapCache>

/*
 * 加载歌词
 */
void MediaPlayer::loadLrcList()
{
    lrcList = playingMusic->getLyricsData();
    emit lrcLoaded();
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    if(lrcList.size() == 0){
        return;
    }

    if(playingLrc != nullptr &&
        playingLrc->startTime <= time && playingLrc->endTime >= time){
        emit playingLrc->update();
    }
    else{
        for(int i=0; i<lrcList.size(); i++){
            if(lrcList[i]->startTime <= time && lrcList[i]->endTime >= time){
                //改变 qml
                playingLrc = lrcList[i];
                emit lrcList[i]->update();
                emit playingLrcIdChange();
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

void MediaPlayer::clearData()
{
    while (!lrcList.empty()) {
        delete lrcList.takeFirst();
    }

    while (!musicList.empty()) {
        delete musicList.takeFirst();
    }

    allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
}

/*
 * 将歌词添加到播放下一首
*/
void MediaPlayer::playingInsertMusic(int coreId)
{
    Music *music = core->musicList[coreId];
    musicList.insert(playingMusicListId, music);

    emit cppPlayingInsertMusic(playingMusicListId);
}

/*
 * 将歌曲插入播放列表
*/
void MediaPlayer::musicInsertPlayingTable(int coreId)
{
    Music *music = core->musicList[coreId];

    musicList.append(music);

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
        musicList.append(core->tableList[table]->showMusics);
        emit cppBuildPlayingTable();
    }

    playingMusic = musicList[music];
    player->setSource(musicList[music]->url);
    player->play();

    emit downLrc(playingMusic->getBaseName(), playingMusic->getLrcUrl());//加载新歌词
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
        aim = playingMusicListId + forward;
        break;
    case 1:
        aim = QRandomGenerator::global()->bounded(max);
        break;
    default:
        aim = playingMusicListId;
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
    playingMusic = nullptr;
    playingLrc = nullptr;
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

LrcData *MediaPlayer::getPlayingLrc() const
{
    return playingLrc;
}


Music *MediaPlayer::getPlayingMusic() const
{
    return playingMusic;
}

QList<LrcData *> MediaPlayer::getLrcList() const
{
    return lrcList;
}

QAudioOutput *MediaPlayer::getAudioOutput() const
{
    return audioOutput;
}

QVector<double> MediaPlayer::getAllSamples() const
{
    return allSamples;
}

QList<Music *> MediaPlayer::getMusicList() const
{
    return musicList;
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


