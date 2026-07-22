#include "AlbumLibraryModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"

AlbumLibraryModel::AlbumLibraryModel(QObject *parent) : QAbstractListModel(parent) {
}

int AlbumLibraryModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_albumList.size();
}

QVariant AlbumLibraryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_albumList.size())
        return QVariant();

    const AlbumPtr album = m_albumList[index.row()];
    switch (role) {
        case IdRole:
            return album->getId();
        case NameRole:
            return album->getName();
        case NameKeyRole:
            return album->getNameKey();
        case DurationRole:
            return album->getDuration();
        case FirstMusicRole:
            return album->getFirstMusic();
        case MusicCountRole:
            return album->getMusicCount();
        case SortRole:
            return album->getSort();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> AlbumLibraryModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[NameKeyRole] = "nameKey";
    roles[DurationRole] = "duration";
    roles[FirstMusicRole] = "firstMusic";
    roles[MusicCountRole] = "musicCount";
    roles[SortRole] = "sort";
    return roles;
}

void AlbumLibraryModel::updateData(const QList<int> &albumIds) {
    QList<AlbumPtr> newList;
    newList.reserve(albumIds.size());

    for (int id : albumIds) {
        AlbumPtr album = DataActive::getInstance().getAlbumCore(id);
        if (album) {
            newList.append(album);
        }
    }

    beginResetModel();
    m_albumList = newList;
    endResetModel();
}

void AlbumLibraryModel::loadByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().getAlbumByKey(key, size, start);
    updateData(ids);
}

void AlbumLibraryModel::loadMoreByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().getAlbumByKey(key, size, start);
    appendAlbumList(ids);
}

void AlbumLibraryModel::loadRandList() {
    QList<int> ids = SQLite::getInstance().getAlbumRandList();
    updateData(ids);
}

void AlbumLibraryModel::appendAlbumList(const QList<int> &albumIds) {
    if (albumIds.isEmpty())
        return;

    QList<AlbumPtr> newAlbums;
    newAlbums.reserve(albumIds.size());

    for (int id : albumIds) {
        AlbumPtr album = DataActive::getInstance().getAlbumCore(id);
        if (album) {
            newAlbums.append(album);
        }
    }

    if (newAlbums.isEmpty())
        return;

    beginInsertRows(QModelIndex(), m_albumList.size(), m_albumList.size() + newAlbums.size() - 1);
    m_albumList.append(newAlbums);
    endInsertRows();
}

void AlbumLibraryModel::clear() {
    beginResetModel();
    m_albumList.clear();
    endResetModel();
}