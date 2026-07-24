#include "ArtistLibraryModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"

ArtistLibraryModel::ArtistLibraryModel(QObject *parent) : QAbstractListModel(parent) {
}

int ArtistLibraryModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_artistList.size();
}

QVariant ArtistLibraryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_artistList.size())
        return QVariant();

    const ArtistPtr artist = m_artistList[index.row()];
    switch (role) {
        case IdRole:
            return artist->getId();
        case NameRole:
            return artist->getName();
        case NameKeyRole:
            return artist->getNameKey();
        case DurationRole:
            return artist->getDuration();
        case FirstMusicRole:
            return artist->getFirstMusic();
        case MusicCountRole:
            return artist->getMusicCount();
        case SortRole:
            return artist->getSort();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ArtistLibraryModel::roleNames() const {
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

void ArtistLibraryModel::updateData(const QList<int> &artistIds) {
    QList<ArtistPtr> newList;
    newList.reserve(artistIds.size());

    for (int id : artistIds) {
        ArtistPtr artist = DataActive::getInstance().getArtistCore(id);
        if (artist) {
            newList.append(artist);
        }
    }

    beginResetModel();
    m_artistList = newList;
    endResetModel();
}

void ArtistLibraryModel::loadByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().artistRepository.getByKey(key, size, start);
    updateData(ids);
}

void ArtistLibraryModel::loadMoreByKey(const QString &key, int size, int start) {
    QList<int> ids = SQLite::getInstance().artistRepository.getByKey(key, size, start);
    appendArtistList(ids);
}

void ArtistLibraryModel::loadRandList() {
    QList<int> ids = SQLite::getInstance().artistRepository.getRandList();
    updateData(ids);
}

void ArtistLibraryModel::appendArtistList(const QList<int> &artistIds) {
    if (artistIds.isEmpty())
        return;

    QList<ArtistPtr> newArtists;
    newArtists.reserve(artistIds.size());

    for (int id : artistIds) {
        ArtistPtr artist = DataActive::getInstance().getArtistCore(id);
        if (artist) {
            newArtists.append(artist);
        }
    }

    if (newArtists.isEmpty())
        return;

    beginInsertRows(QModelIndex(), m_artistList.size(), m_artistList.size() + newArtists.size() - 1);
    m_artistList.append(newArtists);
    endInsertRows();
}

void ArtistLibraryModel::clear() {
    beginResetModel();
    m_artistList.clear();
    endResetModel();
}