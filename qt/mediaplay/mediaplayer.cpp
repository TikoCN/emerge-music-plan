#include "mediaplayer.h"
#include <QRandomGenerator>
#include <QTimer>
#include <utility>

void MediaPlayer::playMusicByListId(const int musicListId)
{
    if (musicListId >= m_musicList.size() || musicListId < 0)
        return;

    m_playingMusic = m_dataActive->getMusicCore(m_musicList[musicListId]);
    m_playingMusic->playNumber++;
    m_playingMusicId = m_playingMusic->id;
    m_player->setSource(m_playingMusic->url);
    m_PlayingListId = musicListId;
    loadLrcList(m_playingMusicId);
}


/*
 * 下一目标
 */
void MediaPlayer::playNext(const int forward)
{
    const long long max = m_musicList.size();
    long long aim;

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

QString MediaPlayer::getTimeString() const {
    QDateTime time;
    time.setMSecsSinceEpoch(m_player->position());
    return time.toString("mm:ss.zzz");
}


MediaPlayer::MediaPlayer(BaseTool *baseTool, DataActive *dataActive)
    :LrcDataControl(baseTool, dataActive)
{
    m_loopType = 0;

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus staus){
        switch (staus) {
        case QMediaPlayer::EndOfMedia:
            playNext(1);
            break;
        case QMediaPlayer::LoadedMedia:
            QTimer::singleShot(50, this, [=](){
                m_player->play();
            });
            break;
        default:
            break;
        }
    });
}

int MediaPlayer::getLoopType() const
{
    return m_loopType;
}

void MediaPlayer::setLoopType(const int newLoopType)
{
    if (m_loopType == newLoopType)
        return;
    m_loopType = newLoopType;
    emit loopTypeChanged();
}

void MediaPlayer::buildPlayingListByMusicList(QList<int> list, const int playMusicInListId)
{
    m_musicList = std::move(list);
    emit musicListBuild();

    playMusicByListId(playMusicInListId);
}

void MediaPlayer::buildPlayingListByMusicId(const int musicId)
{
    QList<int> list;
    list.append(musicId);
    buildPlayingListByMusicList(list);
}

void MediaPlayer::insertPlayingListByMusicList(const QList<int>& list)
{
    const QList<int> leftList = m_musicList.sliced(0, m_PlayingListId);
    const QList<int> rightList = m_musicList.sliced(m_PlayingListId);

    m_musicList.clear();
    m_musicList.append(leftList);
    m_musicList.append(list);
    m_musicList.append(rightList);

    emit musicListBuild();
}

void MediaPlayer::insertPlayingListByMusicId(const int musicId)
{
    QList<int> list;
    list.append(musicId);
    insertPlayingListByMusicList(list);
}

void MediaPlayer::appendPlayingListByMusicList(const QList<int>& list)
{
    m_musicList.append(list);
    emit musicListBuild();
}

void MediaPlayer::appendPlayingListByMusicId(const int musicId)
{
    QList<int> list;
    list.append(musicId);
    appendPlayingListByMusicList(list);
}

QList<int> MediaPlayer::musicList() const
{
    return m_musicList;
}

int MediaPlayer::playingMusicId() const
{
    return m_playingMusicId;
}


