#include "fftw3.h"
#include "mediaplayer.h"
#include "base.h"
#include "sqlite/sqlite.h"
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
    while (!m_lrcList.empty()) {
        delete m_lrcList.takeFirst();
    }

    m_lrcList = m_playingMusic->getLyricsData();
    if (m_lrcList.size() > 0) {
        m_playingLrc->copy(m_lrcList.first());
    }
    emit lrcLoaded();
}

void MediaPlayer::insertMusicAlbum(int albumId)
{
    Base *base = Base::getInstance();

    Album *album = m_core->getAlbum(albumId);
    if (album == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    insertMusicList(album->musicList);
}

void MediaPlayer::insertMusicArtist(int artistId)
{
    Base *base = Base::getInstance();

    Artist *artist = m_core->getArtist(artistId);
    if (artist == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    insertMusicList(artist->musicList);
}

void MediaPlayer::insertMusicTable(int artistId)
{
    Base *base = Base::getInstance();

    Table *table = m_core->getTable(artistId);
    if (table == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    insertMusicList(table->musicList);
}

void MediaPlayer::insertMusic(int musicId)
{
    QList<int> list = {musicId};
    insertMusicList(list);
}

void MediaPlayer::insertMusicList(QList<int> list)
{
    QList<Music *> musicList = m_core->getMusic(list);
    for (int i = 0; i < musicList.size(); ++i) {
        m_musicList.insert(m_playingMusicListId+i, musicList[i]);
    }
    emit musicListBuild();
}

void MediaPlayer::appendMusicAlbum(int albumId)
{
    Base *base = Base::getInstance();

    Album *album = m_core->getAlbum(albumId);
    if (album == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    appendMusicList(album->musicList);
}

void MediaPlayer::appendMusicArtist(int artistId)
{
    Base *base = Base::getInstance();

    Artist *artist = m_core->getArtist(artistId);
    if (artist == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    appendMusicList(artist->musicList);
}

void MediaPlayer::appendMusicTable(int artistId)
{
    Base *base = Base::getInstance();

    Table *table = m_core->getTable(artistId);
    if (table == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    appendMusicList(table->musicList);
}

void MediaPlayer::appendMusic(int musicId)
{
    QList<int> list = {musicId};
    appendMusicList(list);
}

void MediaPlayer::appendMusicList(QList<int> list)
{
    int last = m_musicList.size();
    QList <Music *> musicList = m_core->getMusic(list);
    m_musicList.append(musicList);
    emit musicListAppend(last, musicList.size());
}

void MediaPlayer::selectPlayLrc(qint64 time)
{
    if(m_lrcList.size() == 0){
        return;
    }

    if(m_playingLrc->startTime <= time && m_playingLrc->endTime >= time){
        emit m_playingLrc->update();

        if (m_playingLrc->id >=0 && m_playingLrc->id < m_lrcList.size()) {
            emit m_lrcList[m_playingLrc->id]->update();
        }
    }
    else{
        for(int i=0; i<m_lrcList.size(); i++){
            if(m_lrcList[i]->startTime <= time && m_lrcList[i]->endTime >= time){
                //改变 qml
                m_playingLrc->copy(m_lrcList[i]);
                emit m_lrcList[i]->update();
                emit playingLrcIdChange();
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

void MediaPlayer::clearData()
{
    while (!m_lrcList.empty()) {
        delete m_lrcList.takeFirst();
    }

    while (!m_musicList.empty()) {
        delete m_musicList.takeFirst();
    }

    m_allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
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

void MediaPlayer::buildMusicAlbum(int albumId, int listId)
{
    Base *base = Base::getInstance();

    Album *album = m_core->getAlbum(albumId);
    if (album == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    buildMusicList(album->musicList);
    playMusicList(listId);
}

void MediaPlayer::buildMusicArtist(int artistId, int listId)
{
    Base *base = Base::getInstance();

    Artist *artist = m_core->getArtist(artistId);
    if (artist == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    buildMusicList(artist->musicList);
    playMusicList(listId);
}

void MediaPlayer::buildMusicTable(int tableId, int listId)
{
    Base *base = Base::getInstance();

    Table *table = m_core->getTable(tableId);
    if (table == nullptr) {
        base->sendMessage(tr("找不到专辑，无法播放"));
        return;
    }

    buildMusicList(table->musicList);
    playMusicList(listId);
}

void MediaPlayer::buildMusic(int musicId)
{
    QList<int> list = {musicId};
    buildMusicList(list);
    playMusicList(0);
}

void MediaPlayer::buildMusicList(QList<int> list)
{
    m_musicList = m_core->getMusic(list);
    emit musicListBuild();
}

void MediaPlayer::playMusicList(int musicListId)
{
    if (musicListId >= m_musicList.size() || musicListId < 0)
        return;
    m_playingMusic = m_musicList[musicListId];
    m_playingMusic->setPlayNumber(m_playingMusic->playNumber++);
    m_player->setSource(m_playingMusic->url);
    m_playingMusicListId = musicListId;
    m_player->play();
    loadLrcList();
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
        aim = m_playingMusicListId + forward;

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
        aim = m_playingMusicListId;
        break;
    }

    playMusicList(aim);
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
    m_playingLrc = nullptr;
    m_player = new QMediaPlayer;//播放设备
    m_audioOutput = new QAudioOutput;//音频输出
    m_bufferOutput = new QAudioBufferOutput;//缓冲区输出

    m_playingLrc = new LrcData;
    m_playingLrc->append(0, 1, tr("这是行歌词"));

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    m_core = MusicCore::getInstance();

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &MediaPlayer::buildFrequencySpectrum);

    connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 time){
        updateAudioOutPut();
        selectPlayLrc(time);
    });
    //自动播放

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus staus){
        if(staus == QMediaPlayer::EndOfMedia)
        {
            playNext(1);
        }
    });
}

LrcData *MediaPlayer::getPlayingLrc() const
{
    return m_playingLrc;
}


Music *MediaPlayer::getPlayingMusic() const
{
    return m_playingMusic;
}

QList<LrcData *> MediaPlayer::getLrcList() const
{
    return m_lrcList;
}

QAudioOutput *MediaPlayer::getAudioOutput() const
{
    return m_audioOutput;
}

QVector<double> MediaPlayer::getAllSamples() const
{
    return m_allSamples;
}

QList<Music *> MediaPlayer::getMusicList() const
{
    return m_musicList;
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


