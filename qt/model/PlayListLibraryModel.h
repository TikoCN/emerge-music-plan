#ifndef PLAYLISTLIBRARYMODEL_H
#define PLAYLISTLIBRARYMODEL_H

#include <QAbstractListModel>
#include "baseclass/Playlist.h"

class PlayListLibraryModel : public QAbstractListModel {
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

    explicit PlayListLibraryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void clear();

private:
    QList<PlayListPtr> m_playlistList;
};

#endif // PLAYLISTLIBRARYMODEL_H