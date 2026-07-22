#ifndef ARTISTLIBRARYMODEL_H
#define ARTISTLIBRARYMODEL_H

#include <QAbstractListModel>
#include "baseclass/Artist.h"

class ArtistLibraryModel : public QAbstractListModel {
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

    explicit ArtistLibraryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadMoreByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadRandList();
    Q_INVOKABLE void appendArtistList(const QList<int> &artistIds);
    Q_INVOKABLE void clear();

private:
    QList<ArtistPtr> m_artistList;
    void updateData(const QList<int> &artistIds);
};

#endif // ARTISTLIBRARYMODEL_H