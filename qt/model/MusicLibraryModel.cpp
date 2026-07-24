#include "MusicLibraryModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"
#include "library/MusicLibrary.h"

MusicLibraryModel::MusicLibraryModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int MusicLibraryModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_musicList.size();
}

QVariant MusicLibraryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_musicList.size())
        return QVariant();

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
            return QVariant();
    }
}

QHash<int, QByteArray> MusicLibraryModel::roleNames() const {
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

void MusicLibraryModel::updateData(const QList<int> &musicIds) {
    QList<MusicPtr> newList;
    newList.reserve(musicIds.size());

    for (int id: musicIds) {
        MusicPtr music = DataActive::getInstance().getMusicCore(id);
        if (music) {
            newList.append(music);
        }
    }

    beginResetModel();
    m_musicList = newList;
    endResetModel();
}

void MusicLibraryModel::loadByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().musicRepository.getByKey(key, size, start);
    updateData(ids);
}

void MusicLibraryModel::loadMoreByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().musicRepository.getByKey(key, size, start);
    if (ids.isEmpty()) {
        MusicLibrary::getInstance().loader()->setLoadFinish(true);
        MusicLibrary::getInstance().loader()->setLoadEnable(false);
    }
    appendMusicList(ids);
}

void MusicLibraryModel::loadAlbumMusic(int albumId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().albumRepository.getMusic(albumId, size, start, sort);
    updateData(ids);
}

void MusicLibraryModel::loadMoreAlbumMusic(int albumId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().albumRepository.getMusic(albumId, size, start, sort);
    appendMusicList(ids);
}

void MusicLibraryModel::loadArtistMusic(int artistId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().artistRepository.getMusic(artistId, size, start, sort);
    updateData(ids);
}

void MusicLibraryModel::loadMoreArtistMusic(int artistId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().artistRepository.getMusic(artistId, size, start, sort);
    appendMusicList(ids);
}

void MusicLibraryModel::loadPlayListMusic(int playlistId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().playListRepository.getMusic(playlistId, size, start, sort);
    updateData(ids);
}

void MusicLibraryModel::loadMorePlayListMusic(int playlistId, int size, int start, int sort) {
    QList<int> ids = SQLite::getInstance().playListRepository.getMusic(playlistId, size, start, sort);
    appendMusicList(ids);
}

void MusicLibraryModel::loadRandList(int length) {
    QList<int> ids = SQLite::getInstance().musicRepository.getRandList(length);
    updateData(ids);
}

void MusicLibraryModel::loadMostNewList() {
    QList<int> ids = SQLite::getInstance().musicRepository.getMostNew();
    updateData(ids);
}

void MusicLibraryModel::loadMostPlayedList() {
    QList<int> ids = SQLite::getInstance().musicRepository.getMostPlayed();
    updateData(ids);
}

void MusicLibraryModel::appendMusicList(const QList<int> &musicIds) {
    if (musicIds.isEmpty())
        return;

    QList<MusicPtr> newMusic;
    newMusic.reserve(musicIds.size());

    for (int id: musicIds) {
        MusicPtr music = DataActive::getInstance().getMusicCore(id);
        if (music) {
            newMusic.append(music);
        }
    }

    if (newMusic.isEmpty())
        return;

    beginInsertRows(QModelIndex(), m_musicList.size(), m_musicList.size() + newMusic.size() - 1);
    m_musicList.append(newMusic);
    endInsertRows();
}

void MusicLibraryModel::clear() {
    beginResetModel();
    m_musicList.clear();
    endResetModel();
}

void MusicLibraryModel::updateMusicLove(int musicId, bool isLove) {
    DataActive::getInstance().updateMusicLove(musicId, isLove);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (m_musicList[i]->getId() == musicId) {
            m_musicList[i]->isLove = isLove;
            emit dataChanged(index(i), index(i), {IsLoveRole});
            break;
        }
    }
}

void MusicLibraryModel::updateMusicLevel(int musicId, int level) {
    DataActive::getInstance().updateMusicLevel(musicId, level);

    for (int i = 0; i < m_musicList.size(); ++i) {
        if (m_musicList[i]->getId() == musicId) {
            m_musicList[i]->level = level;
            emit dataChanged(index(i), index(i), {LevelRole});
            break;
        }
    }
}
