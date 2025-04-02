#include "fftw3.h"
#include "mediaplayer.h"
#include "base.h"
#include <QRandomGenerator>
#include <QAudioDevice>
#include <QPixmapCache>
#include <QAudioBuffer>

/*
 * 加载歌词
 */
void MediaPlayer::loadLrcList()
{
    emit clearLrc();

    // 清空以往资源
    while (!lrcList.empty()) {
        delete lrcList.takeFirst();
    }

    lrcList = playingMusic->getLyricsData();
    if (lrcList.size() > 0) {
        playingLrc->copy(lrcList.first());
    }
    emit lrcLoaded();
}

void MediaPlayer::insertMusic(Music *music)
{
    if (music == nullptr) {
        Base::getInstance()->sendMessage(tr("插入歌曲失败"), 1);
        return;
    }

    musicList.insert(playingMusicListId, music);
    emit buildMusicList();
}

void MediaPlayer::insertMusic(Album *album)
{
    if(album == nullptr){
        Base::getInstance()->sendMessage(tr("插入专辑列表失败"), 1);
        return;
    }
    insertMusic(album->musicList);
}

void MediaPlayer::insertMusic(Artist *artist)
{
    if(artist == nullptr){
        Base::getInstance()->sendMessage(tr("插入作曲家列表失败"), 1);
        return;
    }
    insertMusic(artist->musicList);
}

void MediaPlayer::insertMusic(QList<Music *> musicList)
{
    if (musicList.isEmpty()) {
        Base::getInstance()->sendMessage(tr("插入歌曲失败"), 1);
        return;
    }

    for (int i = 0; i < musicList.size(); ++i) {
        musicList.insert(playingMusicListId, musicList[i]);
    }
    emit buildMusicList();
}

void MediaPlayer::appendMusic(Music *music)
{
    if (music == nullptr) {
        return;
    }

    int start = musicList.size();
    musicList.append(music);
    emit musicAppend(start, 1);
}

void MediaPlayer::appendMusic(Album *album)
{
    if(album == nullptr){
        Base::getInstance()->sendMessage(tr("插入专辑列表失败"), 1);
        return;
    }
    appendMusic(album->musicList);
}

void MediaPlayer::appendMusic(Artist *artist)
{
    if(artist == nullptr){
        Base::getInstance()->sendMessage(tr("插入作曲家列表失败"), 1);
        return;
    }
    appendMusic(artist->musicList);
}

void MediaPlayer::appendMusic(QList<Music *> musicList)
{
    if (musicList.isEmpty()) {
        return;
    }

    int start = this->musicList.size();
    this->musicList.append(musicList);
    emit musicAppend(start, musicList.size());
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    if(lrcList.size() == 0){
        return;
    }

    if(playingLrc->startTime <= time && playingLrc->endTime >= time){
        emit playingLrc->update();

        if (playingLrc->id >=0 && playingLrc->id < lrcList.size()) {
            emit lrcList[playingLrc->id]->update();
        }
    }
    else{
        for(int i=0; i<lrcList.size(); i++){
            if(lrcList[i]->startTime <= time && lrcList[i]->endTime >= time){
                //改变 qml
                playingLrc->copy(lrcList[i]);
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


//播放音乐
void MediaPlayer::playMusic(Table *table, int musicId){
    // 清空正在播放列表
    musicList.clear();


    if (table == nullptr) {
        return;
    }

    musicList.append(table->showMusics);
    emit buildMusicList();

    playMusic(musicId);
}

//播放专辑音乐
void MediaPlayer::playMusic(Album *album, int musicId){
    // 清空正在播放列表
    emit clearLrc();
    musicList.clear();

    if (album == nullptr) {
        return;
    }

    musicList.append(album->musicList);
    emit buildMusicList();

    playMusic(musicId);
}

//播放专辑音乐
void MediaPlayer::playMusic(Artist *artist, int musicId){
    // 清空正在播放列表
    musicList.clear();

    if (artist == nullptr) {
        return;
    }

    musicList.append(artist->musicList);
    emit buildMusicList();

    playMusic(musicId);
}

void MediaPlayer::playMusic(int musicId)
{
    playingMusicListId = musicId;
    playingMusic = musicList[musicId];
    player->setSource(musicList[musicId]->url);
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

        if(forward == 1 && aim >= max){
            aim = 0;
        }
        else if(forward == -1 && aim < 0){
            aim = max - 1;
        }
        break;
    case 1:
        aim = QRandomGenerator::global()->bounded(max);
        break;
    default:
        aim = playingMusicListId;
        break;
    }

    playMusic(aim);
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
        QVector<double>out(all);
        // 创建一个FFTW计划
        fftw_complex *in_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * all));
        fftw_complex *out_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * all));

        // 将std::vector数据复制到fftw_complex数组中
        for (int i = 0; i < all; ++i) {
            in_ptr[i][0] = data[i];
            in_ptr[i][1] = 0.0;
        }

        // 创建计划并计算DFT
        fftw_plan plan = fftw_plan_dft_1d(all, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan);

        // 将结果从fftw_complex数组复制回std::vector
        for (int i = 0; i < all; ++i)
        {
            out[i] = sqrt(out_ptr[i][0]*out_ptr[i][0]+out_ptr[i][1]*out_ptr[i][1]);
        }

        // 清理
        fftw_destroy_plan(plan);
        fftw_free(in_ptr);
        fftw_free(out_ptr);
        data = out;

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

    playingLrc = new LrcData;
    playingLrc->append(0, 1, tr("这是行歌词"));

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


