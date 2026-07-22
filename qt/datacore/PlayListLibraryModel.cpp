#include "PlayListLibraryModel.h"
#include "datacore/DataActive.h"
#include <QJsonDocument>

PlayListLibraryModel::PlayListLibraryModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int PlayListLibraryModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_playlistList.size();
}

QVariant PlayListLibraryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_playlistList.size())
        return QVariant();

    const PlayListPtr playlist = m_playlistList[index.row()];
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

QHash<int, QByteArray> PlayListLibraryModel::roleNames() const {
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

void PlayListLibraryModel::loadAll() {
    QString       jsonStr = SQLite::getInstance().getAllList();
    QJsonDocument doc     = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray    array   = doc.array();

    QList<PlayListPtr> newList;
    for (const QJsonValue &value: array) {
        QJsonObject obj      = value.toObject();
        PlayListPtr playlist = PlayListPtr(new PlayList());
        playlist->id         = obj["id"].toInt();
        playlist->name       = obj["name"].toString();
        playlist->url        = obj["url"].toString();
        playlist->duration   = obj["duration"].toVariant().toLongLong();
        playlist->isDir      = obj["isDir"].toBool();
        playlist->isShow     = obj["isShow"].toBool();
        playlist->firstMusic = obj["firstMusic"].toInt();
        playlist->musicCount = obj["musicCount"].toInt();
        playlist->sort       = obj["sort"].toInt();
        newList.append(playlist);
    }

    beginResetModel();
    m_playlistList = newList;
    endResetModel();
}

void PlayListLibraryModel::clear() {
    beginResetModel();
    m_playlistList.clear();
    endResetModel();
}
