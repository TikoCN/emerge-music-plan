#include "ArtistModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"

ArtistModel::ArtistModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int ArtistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_artistList.size();
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const {
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

QHash<int, QByteArray> ArtistModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole]         = "id";
    roles[NameRole]       = "name";
    roles[NameKeyRole]    = "nameKey";
    roles[DurationRole]   = "duration";
    roles[FirstMusicRole] = "firstMusic";
    roles[MusicCountRole] = "musicCount";
    roles[SortRole]       = "sort";
    return roles;
}

void ArtistModel::fetchMore(const QModelIndex &parent) {
    Q_UNUSED(parent);
    QList<int> list;
    switch (type) {
        case AlbumModel:
            list = SQLite::getInstance().albumRepository.getByKey(key, m_loader.limit, m_loader.offset);
            break;
        case KeyModel:
            break;
        case RandModel:
            list = SQLite::getInstance().albumRepository.getRandList();
            break;
    }

    m_loader.offset = static_cast<int>(list.size());
    if (m_loader.offset < 20) {
        m_loader.isFinish = true;
    }

    QList<ArtistPtr> newList;
    newList.reserve(list.size());

    for (const int id: list) {
        ArtistPtr core = DataActive::getInstance().getArtistCore(id);
        if (core) {
            newList.append(core);
        }
    }
    if (newList.isEmpty()) {
        return;
    }
    const int start = rowCount();
    const int end   = std::max(start, start + static_cast<int>(newList.size()) - 1);
    beginInsertRows(QModelIndex(), start, end);
    m_artistList.append(newList);
    endInsertRows();
}

bool ArtistModel::canFetchMore(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return !m_loader.isFinish;
}


void ArtistModel::clear() {
    beginResetModel();
    m_artistList.clear();
    m_loader.reset();
    endResetModel();
}
