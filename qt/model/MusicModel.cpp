#include "MusicModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"
#include "library/MusicLibrary.h"
#include "mediaplay/Mediaplayer.h"

MusicModel::MusicModel(QObject *parent)
    : QAbstractListModel(parent),
      type(KeyModel),
      artistId(-1),
      albumId(-1),
      playlistId(-1),
      key(),
      sort(SORT_TITTLE_ASC) {
    connect(this, &MusicModel::keyChanged, this, &MusicModel::reloadKey);
}

int MusicModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_musicList.size();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_musicList.size())
        return {};

    const MusicPtr music = m_musicList[index.row()];
    if (music.isNull()) {
        switch (role) {
            case IdRole:
            case DurationRole:
            case LastEditRole:
            case LastEditTimeRole:
            case InsetTimeRole:
            case LevelRole:
            case PlayNumberRole:
            case IsLoveRole:
                return 0;
            default:
                return QString();
        }
    }
    switch (role) {
        case IdRole:
            return music->getId();
        case TitleRole:
            return music->getTitle();
        case ArtistRole:
            return music->getArtist();
        case AlbumRole:
            return music->getAlbum();
        case UrlRole:
            return music->getUrl();
        case DurationRole:
            return music->getDuration();
        case LastEditRole:
            return music->getLastEdit();
        case NameKeyRole:
            return music->getNameKey();
        case LastEditTimeRole:
            return music->getLastEditTime();
        case InsetTimeRole:
            return music->getInsetTime();
        case LevelRole:
            return music->getLevel();
        case PlayNumberRole:
            return music->getPlayNumber();
        case IsLoveRole:
            return music->getIsLove();
        default:
            return {};
    }
}

QHash<int, QByteArray> MusicModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole]           = "id";
    roles[TitleRole]        = "title";
    roles[ArtistRole]       = "artist";
    roles[AlbumRole]        = "album";
    roles[UrlRole]          = "url";
    roles[DurationRole]     = "duration";
    roles[LastEditRole]     = "lastEdit";
    roles[NameKeyRole]      = "nameKey";
    roles[LastEditTimeRole] = "lastEditTime";
    roles[InsetTimeRole]    = "insetTime";
    roles[LevelRole]        = "level";
    roles[PlayNumberRole]   = "playNumber";
    roles[IsLoveRole]       = "isLove";
    return roles;
}

void MusicModel::clear() {
    beginResetModel();
    m_musicList.clear();
    m_loader.reset();
    m_loadedPages.clear();
    int totalCount = 0;
    switch (type) {
        case AlbumModel:
            totalCount = DataActive::getInstance().getAlbumData(albumId).getMusicCount();
            break;
        case ArtistModel:
            totalCount = DataActive::getInstance().getArtistData(artistId).getMusicCount();
            break;
        case PlaylistModel:
            totalCount = DataActive::getInstance().getPlaylistData(playlistId).getMusicCount();
            break;
        case NowQueueModel:
            totalCount = SQLite::getInstance().queueRepository.getMusic().size();
            break;
        default:
            break;
    }
    m_musicList.resize(totalCount);
    endResetModel();
}

void MusicModel::reloadKey() {
    if (type != KeyModel) {
        return;
    }

    beginResetModel();
    m_musicList.clear();
    m_loader.reset();
    m_loadedPages.clear();
    endResetModel();

    fetchMore(QModelIndex());
}

void MusicModel::loadRange(const int position) {
    if (position < 0 || position >= m_musicList.size()) {
        return;
    }

    const int page = position / m_loader.limit;
    if (m_loadedPages.contains(page)) {
        return;
    }

    const int offset = page * m_loader.limit;
    QList<int> list;
    switch (type) {
        case AlbumModel:
            list = SQLite::getInstance().albumRepository.getMusic(albumId, m_loader.limit, offset, sort);
            break;
        case ArtistModel:
            list = SQLite::getInstance().artistRepository.getMusic(artistId, m_loader.limit, offset, sort);
            break;
        case PlaylistModel:
            list = SQLite::getInstance().playlistRepository.getMusic(playlistId, m_loader.limit, offset, sort);
            break;
        case NowQueueModel:
            list = SQLite::getInstance().queueRepository.getMusic();
            break;
        default:
            return;
    }

    if (type == NowQueueModel) {
        const int count = std::min(static_cast<int>(list.size()), static_cast<int>(m_musicList.size()));
        for (int i = 0; i < count; ++i) {
            m_musicList[i] = DataActive::getInstance().getMusicCore(list[i]);
        }
        const int pageCount = (static_cast<int>(m_musicList.size()) + m_loader.limit - 1) / m_loader.limit;
        for (int i = 0; i < pageCount; ++i) {
            m_loadedPages.insert(i);
        }
        if (count > 0) {
            emit dataChanged(index(0), index(count - 1));
        }
        return;
    }

    const int count = std::min(static_cast<int>(list.size()),
                               static_cast<int>(m_musicList.size()) - offset);
    for (int i = 0; i < count; ++i) {
        m_musicList[offset + i] = DataActive::getInstance().getMusicCore(list[i]);
    }
    m_loadedPages.insert(page);
    if (count > 0) {
        emit dataChanged(index(offset), index(offset + count - 1));
    }
}

void MusicModel::fetchMore(const QModelIndex &parent) {
    if (parent.isValid() || m_loader.isFinish || type == AlbumModel || type == ArtistModel ||
        type == PlaylistModel || type == NowQueueModel) {
        return;
    }

    QList<int> list;
    bool paginated = true;
    switch (type) {
        case RandModel:
            list = SQLite::getInstance().musicRepository.getRandList(-1);
            paginated = false;
            break;
        case KeyModel:
            list = SQLite::getInstance().musicRepository.getByKey(key, m_loader.limit, m_loader.offset);
            break;
        case MostPlayedModel:
            list = SQLite::getInstance().musicRepository.getMostPlayed();
            paginated = false;
            break;
        case MostNewModel:
            list = SQLite::getInstance().musicRepository.getMostNew();
            paginated = false;
            break;
        default:
            return;
    }

    const int fetchedCount = static_cast<int>(list.size());
    m_loader.offset += fetchedCount;
    m_loader.isFinish = !paginated || fetchedCount < m_loader.limit;

    QList<MusicPtr> newList;
    newList.reserve(list.size());
    for (const int id: list) {
        const MusicPtr music = DataActive::getInstance().getMusicCore(id);
        if (music) {
            newList.append(music);
        }
    }

    if (newList.isEmpty()) {
        return;
    }
    const int start = rowCount(QModelIndex());
    const int end = start + static_cast<int>(newList.size()) - 1;
    beginInsertRows(QModelIndex(), start, end);
    m_musicList.append(newList);
    endInsertRows();
}

bool MusicModel::canFetchMore(const QModelIndex &parent) const {
    return !parent.isValid() && !m_loader.isFinish && type != AlbumModel && type != ArtistModel &&
           type != PlaylistModel && type != NowQueueModel;
}

void MusicModel::updateMusicLove(const int musicId, const bool isLove) {
    DataActive::getInstance().updateMusicLove(musicId, isLove);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (!m_musicList[i].isNull() && m_musicList[i]->getId() == musicId) {
            m_musicList[i]->isLove = isLove;
            emit dataChanged(index(i), index(i), {IsLoveRole});
            break;
        }
    }
}

void MusicModel::updateMusicLevel(int musicId, int level) {
    DataActive::getInstance().updateMusicLevel(musicId, level);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (!m_musicList[i].isNull() && m_musicList[i]->getId() == musicId) {
            m_musicList[i]->level = level;
            emit dataChanged(index(i), index(i), {LevelRole});
            break;
        }
    }
}
