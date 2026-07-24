#ifndef ALBUMLIBRARYMODEL_H
#define ALBUMLIBRARYMODEL_H

#include <QAbstractListModel>
#include "baseclass/Album.h"

class AlbumLibraryModel : public QAbstractListModel {
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

    explicit AlbumLibraryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadMoreByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadRandList();
    Q_INVOKABLE void appendAlbumList(const QList<int> &albumIds);
    Q_INVOKABLE void clear();

private:
    QList<AlbumPtr> m_albumList;
    void updateData(const QList<int> &albumIds);
};

#endif // ALBUMLIBRARYMODEL_H