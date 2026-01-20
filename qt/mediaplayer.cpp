#include "fftw3.h"
#include "mediaplayer.h"
#include <QRandomGenerator>
#include <QAudioDevice>
#include <QPixmapCache>
#include <QAudioBuffer>
#include "filemanagement.h"

/*
 * 加载歌词
 */
void MediaPlayer::loadLrcList()
{
    m_lrcList.clear();
    m_lrcList = FileManagement::getInstance()->getMusicLyricsData(m_playingMusicId);
    m_playingLrcId = -1;
    emit lrcLoaded();
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    if (m_lrcList.size() == 0){
        return;
    }

    if ((m_playingLrcId < 0 ||
         m_PlayingListId > m_lrcList.size()) ||
        (m_lrcList[m_playingLrcId]->startTime <= time &&
         m_lrcList[m_playingLrcId]->endTime >= time)
        ){
        for(int i=0; i<m_lrcList.size(); i++){
            if(m_lrcList[i]->startTime <= time && m_lrcList[i]->endTime >= time){
                m_PlayingListId = i;
                emit playingLrcIdChange(i);
                break;
            }
        }
    }
}
void MediaPlayer::turnToLrc(int lrcId)
{
    if(lrcId >=0 && lrcId < m_lrcList.size()){
        m_player->setPosition(m_lrcList[lrcId]->startTime);
    }
}

QJsonObject MediaPlayer::getLrcJsonObject(int lrcId)
{
    if (lrcId < 0 || lrcId > m_lrcList.size()) {
        return QJsonObject();
    }
    return m_lrcList[lrcId]->getJsonObject();
}

void MediaPlayer::clearData()
{
    m_lrcList.clear();
    m_musicList.clear();
    m_allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
}

void MediaPlayer::clearMusicList()
{
    m_musicList.clear();
    QList<int> idList;
    for (int i = 0; i < m_musicList.size(); ++i) {
        idList.append(m_musicList[i]);
    }
}

void MediaPlayer::updateAudioOutPut()
{
    QAudioOutput* nowOut = new QAudioOutput;
    if(m_audioOutput->device().id() != nowOut->device().id())
    {
        m_audioOutput->setDevice(nowOut->device());


        m_player->setAudioOutput(m_audioOutput);
    }
    delete nowOut;
}

void MediaPlayer::buildPlayingListByMusicList(QList<int> list, int playMusicListId)
{
    clearMusicList();
    m_musicList = list;
    emit musicListBuild();

    playMusicByListId(playMusicListId);
}

void MediaPlayer::buildPlayingListByMusicId(int musicId)
{
    QList<int> list;
    list.append(musicId);
    buildPlayingListByMusicList(list);
}

void MediaPlayer::insertPlayingListByMusicList(QList<int> list)
{
    clearMusicList();
    QList<int> leftList = m_musicList.sliced(0, m_PlayingListId);
    QList<int> rightList = m_musicList.sliced(m_PlayingListId);

    m_musicList.clear();
    m_musicList.append(leftList);
    m_musicList.append(list);
    m_musicList.append(rightList);

    emit musicListBuild();
}

void MediaPlayer::insertPlayingListByMusicId(int musicId)
{
    QList<int> list;
    list.append(musicId);
    insertPlayingListByMusicList(list);
}

void MediaPlayer::appendPlayingListByMusicList(QList<int> list)
{
    clearMusicList();
    m_musicList.append(list);
    emit musicListBuild();
}

void MediaPlayer::appendPlayingListByMusicId(int musicId)
{
    QList<int> list;
    list.append(musicId);
    appendPlayingListByMusicList(list);
}

void MediaPlayer::playMusicByListId(int musicListId)
{
    if (musicListId >= m_musicList.size() || musicListId < 0)
        return;

    m_playingMusic = m_dataActive->getMusicCore(m_musicList[musicListId]);
    m_playingMusic->playNumber++;
    m_playingMusicId = m_playingMusic->id;
    m_player->setSource(m_playingMusic->url);
    m_PlayingListId = musicListId;
    loadLrcList();
}

int MediaPlayer::getLrcListLength()
{
    return m_lrcList.size();
}

/*
 * 下一目标
 */
void MediaPlayer::playNext(int forward)
{
    int max = m_musicList.size();
    int aim = -1;

    if(max == 0){
        return;
    }
    switch (m_loopType) {
    case 0:
        aim = m_PlayingListId + forward;

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
        aim = m_PlayingListId;
        break;
    }

    playMusicByListId(aim);
}

QString MediaPlayer::getTimeString()
{
    QDateTime time;
    time.setMSecsSinceEpoch(m_player->position());
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
        if(m_allSamples.size() != sampleList.size()){
            m_allSamples.clear();
            m_allSamples.fill(0.0, sampleList.size());
        }

        //时间平滑函数
        const double smoothConstantDown = 0.08;
        const double smoothConstantUp = 0.8;
        for(int i=0; i<sampleList.size() && i<m_allSamples.size(); i++){
            if(std::isfinite(sampleList[i])){//判断是不是有理数
                if(sampleList[i] > m_allSamples[i]){
                    m_allSamples[i] = smoothConstantUp * sampleList[i] + (1-smoothConstantUp) * m_allSamples[i];
                }
                else{
                    m_allSamples[i] = smoothConstantDown * sampleList[i] + (1-smoothConstantDown) * m_allSamples[i];
                }
            }
        }

        int aim = 120;
        int cell = m_allSamples.size() / aim;
        QVector<double> outSamples;//处理之后的音乐样本
        outSamples.fill(0, aim);
        cell = cell <= 0 ? 1 : cell;
        for(int i=0; i<aim && i * cell < m_allSamples.size(); i++){
            double max = m_allSamples[i * cell];

            for(int j=1; j<cell && i * cell + j < m_allSamples.size(); j++){
                if(max < m_allSamples[i * cell + j]){
                    max = m_allSamples[i * cell + j];
                }
            }
            outSamples[i] = max;
        }
        emit cppDrawLine(outSamples);
    }
}

MediaPlayer::MediaPlayer()
{
    m_loopType = 0;
    m_playingMusic = nullptr;
    m_playingMusicId = -1;
    m_player = new QMediaPlayer;//播放设备
    m_audioOutput = new QAudioOutput;//音频输出
    m_bufferOutput = new QAudioBufferOutput;//缓冲区输出

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    m_dataActive = DataActive::getInstance();

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &MediaPlayer::buildFrequencySpectrum);

    connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 time){
        updateAudioOutPut();
        selectPlayLrc(time);
    });

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus staus){

        switch (staus) {
        case QMediaPlayer::EndOfMedia:
            playNext(1);
            break;
        case QMediaPlayer::LoadedMedia:
            m_player->play();
            break;
        default:
            break;
        }
    });
}

int MediaPlayer::playingLrcId() const
{
    return m_playingLrcId;
}

QList<int> MediaPlayer::musicList() const
{
    return m_musicList;
}

int MediaPlayer::playingMusicId() const
{
    return m_playingMusicId;
}

QAudioOutput *MediaPlayer::getAudioOutput() const
{
    return m_audioOutput;
}

QVector<double> MediaPlayer::getAllSamples() const
{
    return m_allSamples;
}

int MediaPlayer::getLoopType() const
{
    return m_loopType;
}

void MediaPlayer::setLoopType(int newLoopType)
{
    if (m_loopType == newLoopType)
        return;
    m_loopType = newLoopType;
    emit loopTypeChanged();
}

QMediaPlayer *MediaPlayer::getPlayer() const
{
    return m_player;
}


