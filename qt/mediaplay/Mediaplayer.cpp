#include "Mediaplayer.h"
#include <algorithm>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QTimer>
#include <utility>

namespace {

/**
 * @brief 将媒体状态转换为便于日志定位的名称。
 * @param status 媒体状态。
 * @return 状态名称。
 */
QString mediaStatusName(const QMediaPlayer::MediaStatus status) {
    switch (status) {
        case QMediaPlayer::NoMedia:
            return "NoMedia";
        case QMediaPlayer::LoadingMedia:
            return "LoadingMedia";
        case QMediaPlayer::LoadedMedia:
            return "LoadedMedia";
        case QMediaPlayer::StalledMedia:
            return "StalledMedia";
        case QMediaPlayer::BufferingMedia:
            return "BufferingMedia";
        case QMediaPlayer::BufferedMedia:
            return "BufferedMedia";
        case QMediaPlayer::EndOfMedia:
            return "EndOfMedia";
        case QMediaPlayer::InvalidMedia:
            return "InvalidMedia";
    }
    return "Unknown";
}

/**
 * @brief 将播放状态转换为便于日志定位的名称。
 * @param state 播放状态。
 * @return 状态名称。
 */
QString playbackStateName(const QMediaPlayer::PlaybackState state) {
    switch (state) {
        case QMediaPlayer::StoppedState:
            return "StoppedState";
        case QMediaPlayer::PlayingState:
            return "PlayingState";
        case QMediaPlayer::PausedState:
            return "PausedState";
    }
    return "Unknown";
}

}

/**
 * @brief 按播放列表位置切换音源，并在相同音源时从头继续播放。
 * @param musicListId 播放列表中的歌曲位置。
 */
void MediaPlayer::playMusicByListId(const int musicListId) {
    TLog::getInstance().logInfo(QString("播放请求 listIndex=%1 当前歌曲ID=%2 当前状态=%3 媒体状态=%4 position=%5 duration=%6")
                                    .arg(musicListId)
                                    .arg(m_playingMusicId)
                                    .arg(playbackStateName(m_player->playbackState()))
                                    .arg(mediaStatusName(m_player->mediaStatus()))
                                    .arg(m_player->position())
                                    .arg(m_player->duration()));
    if (musicListId >= m_musicList.size() || musicListId < 0) {
        TLog::getInstance().logError(QString("播放越界歌曲 %1").arg(musicListId));
        return;
    }

    const MusicPtr music = DataActive::getInstance().getMusicCore(m_musicList[musicListId]);
    if (music.isNull()) {
        TLog::getInstance().logError(QString("无法加载播放列表中的歌曲 %1").arg(musicListId));
        return;
    }

    ++m_playbackRevision;
    m_playingMusic = music;
    m_playingMusic->playNumber++;
    m_playingMusicId = m_playingMusic->id;
    emit playingMusicIdChanged();
    m_playingMusicListId = musicListId;
    const QUrl source = QUrl::fromLocalFile(m_playingMusic->url);
    m_startPending = true;
    TLog::getInstance().logInfo(QString("切换音源 revision=%1 songId=%2 title=%3 listIndex=%4 source=%5 sameSource=%6")
                                    .arg(m_playbackRevision)
                                    .arg(m_playingMusicId)
                                    .arg(m_playingMusic->title)
                                    .arg(musicListId)
                                    .arg(source.toString())
                                    .arg(m_player->source() == source));
    if (m_player->source() == source) {
        // 相邻队列项可能指向同一音源；相同 source 不会重新发出 LoadedMedia。
        m_startPending = false;
        m_player->setPosition(0);
        m_player->play();
        schedulePlaybackStartCheck();
    } else {
        m_player->setSource(source);
    }

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
        TLog::getInstance().logError("自动切歌失败：播放列表为空");
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

    TLog::getInstance().logInfo(QString("切歌目标 forward=%1 loopType=%2 fromIndex=%3 toIndex=%4 queueSize=%5")
                                    .arg(forward)
                                    .arg(static_cast<int>(m_loopType))
                                    .arg(m_playingMusicListId)
                                    .arg(aim)
                                    .arg(max));
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

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](const QMediaPlayer::MediaStatus status) {
        const quint64 revision = m_playbackRevision;
        TLog::getInstance().logInfo(QString("媒体状态变化 revision=%1 songId=%2 status=%3 playbackState=%4 position=%5 duration=%6 source=%7")
                                        .arg(revision)
                                        .arg(m_playingMusicId)
                                        .arg(mediaStatusName(status))
                                        .arg(playbackStateName(m_player->playbackState()))
                                        .arg(m_player->position())
                                        .arg(m_player->duration())
                                        .arg(m_player->source().toString()));
        switch (status) {
            case QMediaPlayer::EndOfMedia:
                // 先让后端完成上一首的停止处理，再切换音源，避免在结束回调中重入播放器。
                QTimer::singleShot(0, this, [this, revision]() {
                    if (revision == m_playbackRevision && m_player->mediaStatus() == QMediaPlayer::EndOfMedia) {
                        TLog::getInstance().logInfo(QString("结束事件确认，执行自动切歌 revision=%1 songId=%2 listIndex=%3")
                                                        .arg(revision)
                                                        .arg(m_playingMusicId)
                                                        .arg(m_playingMusicListId));
                        playNext(1);
                    } else {
                        TLog::getInstance().logInfo(QString("忽略过期结束事件 eventRevision=%1 currentRevision=%2 status=%3")
                                                        .arg(revision)
                                                        .arg(m_playbackRevision)
                                                        .arg(mediaStatusName(m_player->mediaStatus())));
                    }
                });
                break;
            case QMediaPlayer::LoadedMedia:
                // LoadedMedia 也会在停止和跳转时出现，只有待启动请求的目标音源可以触发一次自动播放。
                if (revision == m_playbackRevision && m_player->mediaStatus() == QMediaPlayer::LoadedMedia
                    && m_startPending && !m_playingMusic.isNull()
                    && m_player->source() == QUrl::fromLocalFile(m_playingMusic->url)) {
                    m_startPending = false;
                    TLog::getInstance().logInfo(QString("音源加载完成，调用 play revision=%1 songId=%2 position=%3")
                                                    .arg(revision)
                                                    .arg(m_playingMusicId)
                                                    .arg(m_player->position()));
                    m_player->play();
                    schedulePlaybackStartCheck();
                } else {
                    TLog::getInstance().logInfo(QString("忽略非待启动音源的 LoadedMedia eventRevision=%1 currentRevision=%2 status=%3 source=%4 expectedSource=%5 startPending=%6")
                                                    .arg(revision)
                                                    .arg(m_playbackRevision)
                                                    .arg(mediaStatusName(m_player->mediaStatus()))
                                                    .arg(m_player->source().toString())
                                                    .arg(m_playingMusic.isNull() ? QString() : QUrl::fromLocalFile(m_playingMusic->url).toString())
                                                    .arg(m_startPending));
                }
                break;
            default:
                break;
        }
    });

    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](const QMediaPlayer::PlaybackState state) {
        TLog::getInstance().logInfo(QString("播放状态变化 revision=%1 songId=%2 state=%3 mediaStatus=%4 position=%5 duration=%6")
                                        .arg(m_playbackRevision)
                                        .arg(m_playingMusicId)
                                        .arg(playbackStateName(state))
                                        .arg(mediaStatusName(m_player->mediaStatus()))
                                        .arg(m_player->position())
                                        .arg(m_player->duration()));
    });

    connect(m_player, &QMediaPlayer::durationChanged, this, [this](const qint64 duration) {
        TLog::getInstance().logInfo(QString("音频时长变化 revision=%1 songId=%2 duration=%3 mediaStatus=%4")
                                        .arg(m_playbackRevision)
                                        .arg(m_playingMusicId)
                                        .arg(duration)
                                        .arg(mediaStatusName(m_player->mediaStatus())));
    });

    connect(m_player, &QMediaPlayer::positionChanged, this,
            [this, lastPositionBucket = qint64{-1}](const qint64 position) mutable {
        // 按 5 秒区间记录一次，保留进度轨迹并控制日志体积。
        const qint64 currentBucket = position / 5000;
        if (position == 0 || currentBucket != lastPositionBucket) {
            TLog::getInstance().logInfo(QString("播放位置变化 revision=%1 songId=%2 position=%3 duration=%4 state=%5")
                                            .arg(m_playbackRevision)
                                            .arg(m_playingMusicId)
                                            .arg(position)
                                            .arg(m_player->duration())
                                            .arg(playbackStateName(m_player->playbackState())));
            lastPositionBucket = currentBucket;
        }
    });

    QTimer *diagnosticTimer = new QTimer(this);
    diagnosticTimer->setInterval(5000);
    connect(diagnosticTimer, &QTimer::timeout, this, [this]() {
        if (m_player->playbackState() == QMediaPlayer::StoppedState
            && m_player->mediaStatus() != QMediaPlayer::LoadingMedia
            && m_player->mediaStatus() != QMediaPlayer::BufferingMedia
            && m_player->mediaStatus() != QMediaPlayer::StalledMedia) {
            return;
        }
        TLog::getInstance().logInfo(QString("播放心跳 revision=%1 songId=%2 listIndex=%3 state=%4 mediaStatus=%5 position=%6 duration=%7 source=%8")
                                        .arg(m_playbackRevision)
                                        .arg(m_playingMusicId)
                                        .arg(m_playingMusicListId)
                                        .arg(playbackStateName(m_player->playbackState()))
                                        .arg(mediaStatusName(m_player->mediaStatus()))
                                        .arg(m_player->position())
                                        .arg(m_player->duration())
                                        .arg(m_player->source().toString()));
    });
    diagnosticTimer->start();

    connect(m_player, &QMediaPlayer::errorOccurred, this,
            [this](const QMediaPlayer::Error error, const QString &errorString) {
                TLog::getInstance().logError(QString("播放歌曲 %1 失败，音源 %2，错误 %3：%4")
                                             .arg(m_playingMusicId)
                                             .arg(m_player->source().toString())
                                             .arg(static_cast<int>(error))
                                             .arg(errorString));
            });
}

/**
 * @brief 在 MP3 播放已就绪但真实位置仍为零时，通过一次非零跳转恢复启动。
 * @note 检查不会阻塞线程；切歌、暂停、正常推进或不可跳转的音源都会取消恢复。
 */
void MediaPlayer::schedulePlaybackStartCheck() {
    const QUrl source = m_player->source();
    if (QFileInfo(source.toLocalFile()).suffix().compare("mp3", Qt::CaseInsensitive) != 0) {
        return;
    }

    const quint64 revision = m_playbackRevision;
    QTimer::singleShot(1500, this, [this, revision, source]() {
        if (revision != m_playbackRevision || m_player->source() != source
            || m_player->playbackState() != QMediaPlayer::PlayingState
            || m_player->mediaStatus() != QMediaPlayer::BufferedMedia
            || m_player->position() != 0 || m_player->duration() <= 1
            || !m_player->isSeekable() || m_player->error() != QMediaPlayer::NoError) {
            return;
        }

        // 部分 MP3 在缓冲完成后仍停在零位置；零位置跳转可能被后端合并，使用 1 毫秒触发真实跳转。
        TLog::getInstance().logInfo(QString("MP3 启动停滞，执行一次恢复跳转 revision=%1 songId=%2 source=%3 position=%4")
                                        .arg(revision)
                                        .arg(m_playingMusicId)
                                        .arg(source.toString())
                                        .arg(m_player->position()));
        m_player->setPosition(1);

        QTimer::singleShot(1500, this, [this, revision, source]() {
            if (revision != m_playbackRevision || m_player->source() != source) {
                return;
            }
            const QString result = QString("MP3 启动恢复结果 revision=%1 songId=%2 position=%3 state=%4 mediaStatus=%5 source=%6")
                                           .arg(revision)
                                           .arg(m_playingMusicId)
                                           .arg(m_player->position())
                                           .arg(playbackStateName(m_player->playbackState()))
                                           .arg(mediaStatusName(m_player->mediaStatus()))
                                           .arg(source.toString());
            if (m_player->playbackState() == QMediaPlayer::PlayingState && m_player->position() <= 1) {
                TLog::getInstance().logError(result);
            } else {
                TLog::getInstance().logInfo(result);
            }
        });
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

    // 同步的数据库
    SQLite::getInstance().playlistRepository.deletePlayingList(0);
    SQLite::getInstance().playlistRepository.appendPlayingMusic(m_musicList, 0);
    emit musicListBuild();
}

void MediaPlayer::buildPlayingListByMusicId(const int musicId) {
    if (DataActive::getInstance().getMusicCore(musicId) == nullptr)
        return;
    buildPlayingList({musicId}, 0);
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

void MediaPlayer::buildPlayingPlaylist(const int playListId, const int listId, const int sort) {
    const auto playList  = DataActive::getInstance().getPlaylistCore(playListId);
    const int playlistSort = sort < 0 ? playList->sort : sort;
    const auto musicList = SQLite::getInstance().playlistRepository.getMusicAll(playListId, playlistSort);
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

void MediaPlayer::insertPlayingPlaylist(const int playListId) {
    const auto playList  = DataActive::getInstance().getPlaylistCore(playListId);
    const auto musicList = SQLite::getInstance().playlistRepository.getMusicAll(playListId, playList->sort);
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

void MediaPlayer::appendPlayingPlaylist(const int playListId) {
    const auto playList  = DataActive::getInstance().getPlaylistCore(playListId);
    const auto musicList = SQLite::getInstance().playlistRepository.getMusicAll(playListId, playList->sort);
    appendPlayingList(musicList);
}

void MediaPlayer::insertPlayingList(const QList<int> &list) {
    // 下一首位于当前播放位置之后；尚未开始播放时插入到队列开头。
    const int insertIndex = std::clamp(m_playingMusicListId + 1, 0, static_cast<int>(m_musicList.size()));
    m_musicList = m_musicList.sliced(0, insertIndex) + list + m_musicList.sliced(insertIndex);

    // 同步数据库
    SQLite::getInstance().playlistRepository.deletePlayingList(insertIndex);
    SQLite::getInstance().playlistRepository.appendPlayingMusic(m_musicList.sliced(insertIndex), insertIndex);
    emit musicListBuild();
}

void MediaPlayer::insertPlayingListByMusicId(const int musicId) {
    if (DataActive::getInstance().getMusicCore(musicId) == nullptr)
        return;
    insertPlayingList({musicId});
}

/**
 * @brief 追加歌曲到播放列表，并同步数据库
 * 数据库同步统一在此处处理，保证 appendPlayingArtist/Album/Playlist 行为一致
 */
void MediaPlayer::appendPlayingList(const QList<int> &list) {
    const int offset = static_cast<int>(m_musicList.size());
    m_musicList.append(list);

    // 统一在此同步到数据库，各 appendPlaying* 调用方不再单独处理
    SQLite::getInstance().playlistRepository.appendPlayingMusic(list, offset);
    emit musicListBuild();
}

void MediaPlayer::appendPlayingListByMusicId(const int musicId) {
    if (DataActive::getInstance().getMusicCore(musicId) == nullptr)
        return;
    appendPlayingList({musicId});
}

QList<int> MediaPlayer::getMusicList(const int size, const int start) const {
    return m_musicList.mid(start, size);
}

/**
 * @brief 从数据库恢复上次播放列表和位置
 */
void MediaPlayer::initData() {
    m_musicList = SQLite::getInstance().playlistRepository.getPlayingListMusic();

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
