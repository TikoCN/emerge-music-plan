#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <qqmlintegration.h>

#include "baseclass/Playlist.h"
#include "macro/NotifyProperty.h"
#include "datacore/DataLoader.h"

class PlaylistModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum PlayListRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        UrlRole,
        DurationRole,
        IsDirRole,
        IsShowRole,
        FirstMusicRole,
        MusicCountRole,
        SortRole
    };

    enum PlaylistType {
        All,
        User,
        Dir
    };

    Q_ENUM(PlaylistType)
    QT_NOTIFY_PROPERTY(PlaylistType, type, Type);

    explicit PlaylistModel(QObject *parent = nullptr);

    int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QList<int>       loadAll();
    QList<int>       loadUser();
    QList<int>       loadDir();
    Q_INVOKABLE void clear();
    void             fetchMore(const QModelIndex &parent) override;
    bool             canFetchMore(const QModelIndex &parent) const override;

private:
    QList<PlayListPtr> m_playlistList;
    Loader             m_loader;
};

#endif // PLAYLISTMODEL_H
