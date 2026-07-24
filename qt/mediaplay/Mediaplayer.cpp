#include "Mediaplayer.h"
#include <QRandomGenerator>
#include <QTimer>
#include <utility>

void MediaPlayer::playMusicByListId(const int musicListId) {
    if (musicListId >= m_musicList.size() || musicListId < 0) {
        TLog::getInstance().logError(QString("播放越界歌曲 %1").arg(musicListId));
        return;
    }

    m_playingMusic = DataActive::getInstance().getMusicCore(m_musicList[musicListId]);
    m_playingMusic->playNumber++;
    m_playingMusicId = m_playingMusic->id;
    m_player->setSource(m_playingMusic->url);
    m_playingMusicListId = musicListId; // 统一使用修正后的命名

    QString lrcUrl = FileManagement::getMusicLrcUrl(m_playingMusicId);
    if (!QFile::exists(lrcUrl)) {
        emit downLrc(m_playingMusic->title, lrcUrl, m_playingMusicId);
    } else {
        loadLrcList(m_playingMusicId);
    }
    TLog::getInstance().logInfo(QString("播放音乐，正在播放 音乐 listid%1").arg(musicListId));
}


/**
 * @brief 播放下一首歌曲
 * @param forward 前进方向
 */
void MediaPlayer::playNext(const int forward) {
    const int max = static_cast<int>(m_musicList.size());
    int       aim;

    if (max == 0) {
        return;
    }

    // 使用 LoopType 枚举替代魔法数字
    switch (m_loopType) {
        case LoopType::Sequential:
            aim = m_playingMusicListId + forward;
            if (forward == 1 && aim >= max) {
                aim = 0;
            } else if (forward == -1 && aim < 0) {
                aim = max - 1;
            }
            break;
        case LoopType::Random:
            aim = QRandomGenerator::global()->bounded(max);
            break;
        case LoopType::RepeatOne:
        default:
            aim = m_playingMusicListId;
            break;
    }

    playMusicByListId(aim);
}

/**
 * @brief 获得当前播放时间字符串，使用整数运算替代 QDateTime 语义歧义
 */
QString MediaPlayer::getTimeString() const {
    const qint64 pos  = m_player->position(); // 毫秒
    const int    min  = static_cast<int>(pos / 60000);
    const int    sec  = static_cast<int>((pos % 60000) / 1000);
    const int    msec = static_cast<int>(pos % 1000);
    return QString("%1:%2.%3")
        .arg(min,  2, 10, QChar('0'))
        .arg(sec,  2, 10, QChar('0'))
        .arg(msec, 3, 10, QChar('0'));
}

MediaPlayer::MediaPlayer() {
    m_loopType           = LoopType::Sequential;
    m_playingMusicId     = -1;
    m_playingMusicListId = -1;

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
    return static_cast<int>(m_loopType);
}

void MediaPlayer::setLoopType(const int newLoopType) {
    const auto newType = static_cast<LoopType>(newLoopType);
    if (m_loopType == newType)
        return;
    m_loopType = newType;
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
    SQLite::getInstance().playListRepository.deletePlayingList(0);
    SQLite::getInstance().playListRepository.appendPlayingMusic(m_musicList, 0);
}

void MediaPlayer::buildPlayingArtist(const int artistId, const int listId) {
    const auto artist    = DataActive::getInstance().getArtistCore(artistId);
    const auto musicList = SQLite::getInstance().artistRepository.getMusicAll(artistId, artist->sort);
    buildPlayingList(musicList, listId);

    TLog::getInstance().logUser(QString("播放歌手音乐%1").arg(artistId));
}

void MediaPlayer::buildPlayingAlbum(const int albumId, const int listId) {
    const auto album     = DataActive::getInstance().getAlbumCore(albumId);
    const auto musicList = SQLite::getInstance().albumRepository.getMusicAll(albumId, album->sort);
    buildPlayingList(musicList, listId);

    TLog::getInstance().logUser(QString("播放专辑音乐%1").arg(albumId));
}

void MediaPlayer::buildPlayingPlayList(const int playListId, const int listId) {
    const auto playList  = DataActive::getInstance().getPlayListCore(playListId);
    const auto musicList = SQLite::getInstance().playListRepository.getMusicAll(playListId, playList->sort);
    buildPlayingList(musicList, listId);

    TLog::getInstance().logUser(QString("播放列表音乐%1").arg(playListId));
}

void MediaPlayer::insertPlayingArtist(const int artistId) {
    const auto artist    = DataActive::getInstance().getArtistCore(artistId);
    const auto musicList = SQLite::getInstance().artistRepository.getMusicAll(artistId, artist->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::insertPlayingAlbum(const int albumId) {
    const auto album     = DataActive::getInstance().getAlbumCore(albumId);
    const auto musicList = SQLite::getInstance().albumRepository.getMusicAll(albumId, album->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::insertPlayingPlayList(const int playListId) {
    const auto playList  = DataActive::getInstance().getPlayListCore(playListId);
    const auto musicList = SQLite::getInstance().playListRepository.getMusicAll(playListId, playList->sort);
    insertPlayingList(musicList);
}

void MediaPlayer::appendPlayingArtist(const int artistId) {
    const auto artist    = DataActive::getInstance().getArtistCore(artistId);
    const auto musicList = SQLite::getInstance().artistRepository.getMusicAll(artistId, artist->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::appendPlayingAlbum(const int albumId) {
    const auto album     = DataActive::getInstance().getAlbumCore(albumId);
    const auto musicList = SQLite::getInstance().albumRepository.getMusicAll(albumId, album->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::appendPlayingPlayList(const int playListId) {
    const auto playList  = DataActive::getInstance().getPlayListCore(playListId);
    const auto musicList = SQLite::getInstance().playListRepository.getMusicAll(playListId, playList->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::insertPlayingList(const QList<int> &list) {
    const QList<int> leftList  = m_musicList.sliced(0, m_playingMusicListId);
    const QList<int> rightList = std::move(list) + m_musicList.sliced(m_playingMusicListId);

    m_musicList.clear();
    m_musicList.append(leftList);
    m_musicList.append(rightList);
    emit musicListBuild();

    // 同步数据库
    SQLite::getInstance().playListRepository.deletePlayingList(m_playingMusicListId);
    SQLite::getInstance().playListRepository.appendPlayingMusic(rightList, m_playingMusicListId);
}

/**
 * @brief 追加歌曲到播放列表，并同步数据库
 * 数据库同步统一在此处处理，保证 appendPlayingArtist/Album/PlayList 行为一致
 */
void MediaPlayer::appendPlayingList(const QList<int> &list) {
    const int offset = static_cast<int>(m_musicList.size());
    m_musicList.append(list);
    emit musicListBuild();

    // 统一在此同步到数据库，各 appendPlaying* 调用方不再单独处理
    SQLite::getInstance().playListRepository.appendPlayingMusic(list, offset);
}

QList<int> MediaPlayer::getMusicList(const int size, const int start) const {
    return m_musicList.mid(start, size);
}

/**
 * @brief 从数据库恢复上次播放列表和位置
 */
void MediaPlayer::initData() {
    m_musicList = SQLite::getInstance().playListRepository.getPlayingListMusic();

    // 仅在列表非空且位置合法时恢复播放
    if (!m_musicList.isEmpty() && m_playingMusicListId >= 0
        && m_playingMusicListId < m_musicList.size()) {
        playMusicByListId(m_playingMusicListId);
    }
}

void MediaPlayer::setPlayingMusicListId(const int playingMusicListId) {
    m_playingMusicListId = playingMusicListId;
}

int MediaPlayer::getPlayingMusicListId() const {
    return m_playingMusicListId;
}

int MediaPlayer::playingMusicId() const {
    return m_playingMusicId;
}
