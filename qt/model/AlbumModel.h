#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include <QAbstractListModel>
#include "baseclass/Album.h"
#include "datacore/DataLoader.h"
#include "macro/NotifyProperty.h"

class AlbumModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum AlbumRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        NameKeyRole,
        DurationRole,
        FirstMusicRole,
        MusicCountRole,
        SortRole
    };

    enum AlbumType {
        KeyModel,
        ArtistModel,
        RandModel
    };

    Q_ENUM(AlbumType)
    QT_NOTIFY_PROPERTY(AlbumType, type, Type)
    QT_NOTIFY_PROPERTY(QString, key, Key)
    QT_NOTIFY_PROPERTY(int, artistId, ArtistId)

    explicit AlbumModel(QObject *parent = nullptr);

    int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void             fetchMore(const QModelIndex &parent) override;
    bool             canFetchMore(const QModelIndex &parent) const override;
    Q_INVOKABLE void clear();

private:
    QList<AlbumPtr> m_albumList;
    Loader          m_loader;
};

#endif // ALBUMMODEL_H
