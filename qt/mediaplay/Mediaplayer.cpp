#include "Mediaplayer.h"
#include <QRandomGenerator>
#include <QTimer>
#include <utility>

void MediaPlayer::playMusicByListId(const int musicListId) {
    if (musicListId >= m_musicList.size() || musicListId < 0) {
        m_loger->logError(QString("播放越界歌曲 %1").arg(musicListId));
        return;
    }

    m_playingMusic = m_dataActive->getMusicCore(m_musicList[musicListId]);
    m_playingMusic->playNumber++;
    m_playingMusicId = m_playingMusic->id;
    m_player->setSource(m_playingMusic->url);
    m_PlayingMusicListId = musicListId;

    loadLrcList(m_playingMusicId);
    m_loger->logInfo(QString("播放音乐，正在播放 音乐 listid%1").arg(musicListId));
}


/**
 * @brief 播放下一首歌曲
 * @param forward 前进方向
 */
void MediaPlayer::playNext(const int forward) {
    const int max = static_cast<int>(m_musicList.size());
    int aim;

    if (max == 0) {
        return;
    }
    switch (m_loopType) {
        case 0:
            aim = m_PlayingMusicListId + forward;

            if (forward == 1 && aim >= max) {
                aim = 0;
            } else if (forward == -1 && aim < 0) {
                aim = max - 1;
            }
            break;
        case 1:
            aim = QRandomGenerator::global()->bounded(max);
            break;
        default:
            aim = m_PlayingMusicListId;
            break;
    }

    playMusicByListId(aim);
}

QString MediaPlayer::getTimeString() const {
    QDateTime time;
    time.setMSecsSinceEpoch(m_player->position());
    return time.toString("mm:ss.zzz");
}

MediaPlayer::MediaPlayer(BaseTool *baseTool, DataActive *dataActive, TLog *log, SQLite *sql, QObject *parent)
    : LrcDataControl(baseTool, dataActive, log, parent)
      , m_sqlite(sql) {
    m_loopType = 0;
    m_playingMusicId = -1;
    m_PlayingMusicListId = -1;

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](const QMediaPlayer::MediaStatus staus) {
        switch (staus) {
            case QMediaPlayer::EndOfMedia:
                playNext(1);
                break;
            case QMediaPlayer::LoadedMedia:
                QTimer::singleShot(50, this, [this]() {
                    m_player->play();
                });
                break;
            default:
                break;
        }
    });
}

int MediaPlayer::getLoopType() const {
    return m_loopType;
}

void MediaPlayer::setLoopType(const int newLoopType) {
    if (m_loopType == newLoopType)
        return;
    m_loopType = newLoopType;
    emit loopTypeChanged();
}

/**
 * @brief 根据 歌曲id列表 建立正在播放队列，并设置初始播放位置
 * @param list 歌曲id列表
 * @param playMusicInListId 初始播放位置，默认为 0
 */
void MediaPlayer::buildPlayingList(QList<int> list, const int playMusicInListId) {
    m_musicList = std::move(list);
    playMusicByListId(playMusicInListId);
    emit musicListBuild();

    // 同步的数据库
    m_sqlite->deletePlayingList(0);
    m_sqlite->appendPlayingListMusic(m_musicList, 0);
}

void MediaPlayer::buildPlayingArtist(const int artistId, const int listId) {
    const auto artist = m_dataActive->getArtistCore(artistId);
    const auto musicList = m_sqlite->getArtistMusicAll(artistId, artist->sort);
    buildPlayingList(musicList, listId);

    m_loger->logUser(QString("播放歌手音乐%1").arg(artistId));
}

void MediaPlayer::buildPlayingAlbum(const int albumId, const int listId) {
    const auto album = m_dataActive->getAlbumCore(albumId);
    const auto musicList = m_sqlite->getAlbumMusicAll(albumId, album->sort);
    buildPlayingList(musicList, listId);

    m_loger->logUser(QString("播放专辑音乐%1").arg(albumId));
}

void MediaPlayer::buildPlayingPlayList(const int playListId, const int listId) {
    const auto playList = m_dataActive->getPlayListCore(playListId);
    const auto musicList = m_sqlite->getPlayListMusicAll(playListId, playList->sort);
    buildPlayingList(musicList, listId);

    m_loger->logUser(QString("播放列表音乐%1").arg(playListId));
}

void MediaPlayer::insertPlayingArtist(const int artistId) {
    const auto artist = m_dataActive->getArtistCore(artistId);
    const auto musicList = m_sqlite->getArtistMusicAll(artistId, artist->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::insertPlayingAlbum(const int albumId) {
    const auto album = m_dataActive->getAlbumCore(albumId);
    const auto musicList = m_sqlite->getAlbumMusicAll(albumId, album->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::insertPlayingPlayList(const int playListId) {
    const auto playList = m_dataActive->getPlayListCore(playListId);
    const auto musicList = m_sqlite->getPlayListMusicAll(playListId, playList->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::appendPlayingArtist(const int artistId) {
    const auto artist = m_dataActive->getArtistCore(artistId);
    const auto musicList = m_sqlite->getArtistMusicAll(artistId, artist->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::appendPlayingAlbum(const int albumId) {
    const auto album = m_dataActive->getAlbumCore(albumId);
    const auto musicList = m_sqlite->getPlayListMusicAll(albumId, album->sort);
    m_sqlite->updatePlayingListMusic(musicList, static_cast<int>(m_musicList.size()));
    appendPlayingList(musicList);
}

void MediaPlayer::appendPlayingPlayList(const int playListId) {
    const auto playList = m_dataActive->getPlayListCore(playListId);
    const auto musicList = m_sqlite->getPlayListMusicAll(playListId, playList->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::insertPlayingList(const QList<int> &list) {
    const QList<int> leftList = m_musicList.sliced(0, m_PlayingMusicListId);
    const QList<int> rightList = std::move(list) + m_musicList.sliced(m_PlayingMusicListId);

    m_musicList.clear();
    m_musicList.append(leftList);
    m_musicList.append(rightList);
    emit musicListBuild();

    // 同步数据库
    m_sqlite->deletePlayingList(m_PlayingMusicListId);
    m_sqlite->appendPlayingListMusic(rightList, m_PlayingMusicListId);
}

void MediaPlayer::appendPlayingList(const QList<int> &list) {
    const int length = static_cast<int>(list.size());
    m_musicList.append(list);
    emit musicListBuild();

    // 同步到数据库
    m_sqlite->appendPlayingListMusic(list, length);
}

QList<int> MediaPlayer::getMusicList(const int size, const int start) const {
    return m_musicList.mid(start, size);
}

void MediaPlayer::initData() {
    m_musicList = m_sqlite->getPlayingListMusic();
    playMusicByListId(m_PlayingMusicListId);
}

void MediaPlayer::setPlayingMusicListId(const int playingMusicListId) {
    m_PlayingMusicListId = playingMusicListId;
}

int MediaPlayer::getPlayingMusicListId() const {
    return m_PlayingMusicListId;
}

int MediaPlayer::playingMusicId() const {
    return m_playingMusicId;
}