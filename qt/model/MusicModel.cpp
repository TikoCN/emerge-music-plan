#include "MusicModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"
#include "library/MusicLibrary.h"
#include "mediaplay/Mediaplayer.h"

MusicModel::MusicModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int MusicModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_musicList.size();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_musicList.size())
        return {};

    const MusicPtr music = m_musicList[index.row()];
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
    endResetModel();
}

void MusicModel::fetchMore(const QModelIndex &parent) {
    Q_UNUSED(parent);
    QList<int> list;
    switch (type) {
        case AlbumModel:
            list = SQLite::getInstance().albumRepository.getMusic(albumId, m_loader.limit, m_loader.offset, sort);
            break;
        case ArtistModel:
            list = SQLite::getInstance().artistRepository.getMusic(artistId, m_loader.limit, m_loader.offset, sort);
            break;
        case PlaylistModel:
            list = SQLite::getInstance().playlistRepository.getMusic(playlistId, m_loader.limit, m_loader.offset, sort);
            break;
        case RandModel:
            list = SQLite::getInstance().musicRepository.getRandList(-1);
            break;
        case KeyModel:
            list = SQLite::getInstance().musicRepository.getByKey(key, m_loader.limit, m_loader.offset);
            break;
        case MostPlayedModel:
            list = SQLite::getInstance().musicRepository.getMostPlayed();
            break;
        case MostNewModel:
            list = SQLite::getInstance().musicRepository.getMostNew();
            break;
        case NowQueueModel:
            list = SQLite::getInstance().queueRepository.getMusic();
            break;
    }
    m_loader.offset = static_cast<int>(list.size());
    if (m_loader.offset < 20) {
        m_loader.isFinish = true;
    }

    QList<MusicPtr> newList;
    newList.reserve(list.size());

    for (int id: list) {
        MusicPtr music = DataActive::getInstance().getMusicCore(id);
        if (music) {
            newList.append(music);
        }
    }

    if (newList.isEmpty()) {
        return;
    }
    const int start = rowCount(QModelIndex());
    const int end   = std::max(start, start + static_cast<int>(newList.size()) - 1);
    beginInsertRows(QModelIndex(), start, end);
    m_musicList.append(newList);
    endInsertRows();
}

bool MusicModel::canFetchMore(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return !m_loader.isFinish;
}

void MusicModel::updateMusicLove(const int musicId, const bool isLove) {
    DataActive::getInstance().updateMusicLove(musicId, isLove);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (m_musicList[i]->getId() == musicId) {
            m_musicList[i]->isLove = isLove;
            emit dataChanged(index(i), index(i), {IsLoveRole});
            break;
        }
    }
}

void MusicModel::updateMusicLevel(int musicId, int level) {
    DataActive::getInstance().updateMusicLevel(musicId, level);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (m_musicList[i]->getId() == musicId) {
            m_musicList[i]->level = level;
            emit dataChanged(index(i), index(i), {LevelRole});
            break;
        }
    }
}
