#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QAbstractListModel>
#include "baseclass/Artist.h"
#include "datacore/DataLoader.h"
#include "macro/NotifyProperty.h"

class ArtistModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum ArtistRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        NameKeyRole,
        DurationRole,
        FirstMusicRole,
        MusicCountRole,
        SortRole
    };

    enum ArtistType {
        KeyModel,
        AlbumModel,
        RandModel
    };

    Q_ENUM(ArtistType)
    QT_NOTIFY_PROPERTY(ArtistType, type, Type)
    QT_NOTIFY_PROPERTY(QString, key, Key)
    QT_NOTIFY_PROPERTY(int, albumId, AlbumId)

    explicit ArtistModel(QObject *parent = nullptr);

    int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void             fetchMore(const QModelIndex &parent) override;
    bool             canFetchMore(const QModelIndex &parent) const override;
    Q_INVOKABLE void clear();

private:
    QList<ArtistPtr> m_artistList;
    Loader           m_loader;
};

#endif // ARTISTMODEL_H
