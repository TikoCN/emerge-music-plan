#include "AlbumModel.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"

AlbumModel::AlbumModel(QObject *parent)
    : QAbstractListModel(parent) {
    connect(this, &AlbumModel::keyChanged, this, &AlbumModel::reloadKey);
}

int AlbumModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_albumList.size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const {
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

QHash<int, QByteArray> AlbumModel::roleNames() const {
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

void AlbumModel::fetchMore(const QModelIndex &parent) {
    if (parent.isValid() || m_loader.isFinish) {
        return;
    }

    QList<int> list;
    switch (type) {
        case ArtistModel:
            list = SQLite::getInstance().albumRepository.getByKey(key, m_loader.limit, m_loader.offset);
            break;
        case KeyModel:
            list = SQLite::getInstance().albumRepository.getByKey(key, m_loader.limit, m_loader.offset);
            break;
        case RandModel:
            list = SQLite::getInstance().albumRepository.getRandList();
            break;
    }

    m_loader.offset += static_cast<int>(list.size());
    m_loader.isFinish = list.size() < m_loader.limit;

    QList<AlbumPtr> newList;
    newList.reserve(list.size());

    for (const int id: list) {
        AlbumPtr core = DataActive::getInstance().getAlbumCore(id);
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
    m_albumList.append(newList);
    endInsertRows();
}

bool AlbumModel::canFetchMore(const QModelIndex &parent) const {
    return !parent.isValid() && !m_loader.isFinish;
}

void AlbumModel::reloadKey() {
    if (type != KeyModel) {
        return;
    }

    clear();
    fetchMore(QModelIndex());
}

void AlbumModel::clear() {
    beginResetModel();
    m_albumList.clear();
    m_loader.reset();
    endResetModel();
}
