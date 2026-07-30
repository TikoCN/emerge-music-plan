#include "PlaylistModel.h"

#include <qimageiohandler.h>

#include "datacore/DataActive.h"
#include <QJsonDocument>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_playlistList.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_playlistList.size())
        return QVariant();

    const PlaylistPtr playlist = m_playlistList[index.row()];
    switch (role) {
        case IdRole:
            return playlist->getId();
        case NameRole:
            return playlist->getName();
        case UrlRole:
            return playlist->getUrl();
        case DurationRole:
            return playlist->getDuration();
        case IsDirRole:
            return playlist->getIsDir();
        case IsShowRole:
            return playlist->getIsShow();
        case FirstMusicRole:
            return playlist->getFirstMusic();
        case MusicCountRole:
            return playlist->getMusicCount();
        case SortRole:
            return playlist->getSort();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole]         = "id";
    roles[NameRole]       = "name";
    roles[UrlRole]        = "url";
    roles[DurationRole]   = "duration";
    roles[IsDirRole]      = "isDir";
    roles[IsShowRole]     = "isShow";
    roles[FirstMusicRole] = "firstMusic";
    roles[MusicCountRole] = "musicCount";
    roles[SortRole]       = "sort";
    return roles;
}

QList<int> PlaylistModel::loadAll() {
    const auto dirs  = SQLite::getInstance().playlistRepository.getList(m_loader.offset, m_loader.limit, true);
    const auto users = SQLite::getInstance().playlistRepository.getList(m_loader.offset, m_loader.limit, false);

    const auto list = dirs + users;
    return list;
}

QList<int> PlaylistModel::loadUser() {
    const auto users = SQLite::getInstance().playlistRepository.getList(m_loader.offset, m_loader.limit, false);
    return users;
}

QList<int> PlaylistModel::loadDir() {
    const auto dirs = SQLite::getInstance().playlistRepository.getList(m_loader.offset, m_loader.limit, true);
    return dirs;
}

void PlaylistModel::clear() {
    beginResetModel();
    m_playlistList.clear();
    m_loader.reset();
    endResetModel();
}

void PlaylistModel::fetchMore(const QModelIndex &parent) {
    Q_UNUSED(parent);

    QList<int> list;

    switch (type) {
        case AllModel:
            list = loadAll();
            break;
        case DirModel:
            list = loadDir();
            break;
        case UserModel:
            list = loadUser();
            break;
    }

    m_loader.offset = static_cast<int>(list.size());
    if (m_loader.offset < 20) {
        m_loader.isFinish = true;
    }

    QList<PlaylistPtr> playlistList;
    playlistList.reserve(list.size());

    for (const auto id: list) {
        auto playlist = SQLite::getInstance().playlistRepository.get(id);
        if (playlist) {
            playlistList.append(playlist);
        }
    }
    if (playlistList.isEmpty()) {
        return;
    }
    const int start = rowCount();
    const int end   = std::max(start, start + static_cast<int>(playlistList.size()) - 1);

    beginInsertRows(QModelIndex(), start, end);
    m_playlistList.append(playlistList);
    endInsertRows();
}

bool PlaylistModel::canFetchMore(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return !m_loader.isFinish;
}
