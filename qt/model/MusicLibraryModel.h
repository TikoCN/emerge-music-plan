#ifndef MUSICLIBRARYMODEL_H
#define MUSICLIBRARYMODEL_H

#include <QAbstractListModel>
#include "baseclass/Music.h"

class MusicLibraryModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum MusicRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        UrlRole,
        DurationRole,
        LastEditRole,
        NameKeyRole,
        LastEditTimeRole,
        InsetTimeRole,
        LevelRole,
        PlayNumberRole,
        IsLoveRole
    };

    explicit MusicLibraryModel(QObject *parent = nullptr);

    int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadMoreByKey(const QString &key, int size, int start);
    Q_INVOKABLE void loadAlbumMusic(int albumId, int size, int start, int sort);
    Q_INVOKABLE void loadMoreAlbumMusic(int albumId, int size, int start, int sort);
    Q_INVOKABLE void loadArtistMusic(int artistId, int size, int start, int sort);
    Q_INVOKABLE void loadMoreArtistMusic(int artistId, int size, int start, int sort);
    Q_INVOKABLE void loadPlayListMusic(int playlistId, int size, int start, int sort);
    Q_INVOKABLE void loadMorePlayListMusic(int playlistId, int size, int start, int sort);
    Q_INVOKABLE void loadRandList(int length = -1);
    Q_INVOKABLE void loadMostNewList();
    Q_INVOKABLE void loadMostPlayedList();
    Q_INVOKABLE void appendMusicList(const QList<int> &musicIds);
    Q_INVOKABLE void clear();

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, int level);

private:
    QList<MusicPtr> m_musicList;

    void updateData(const QList<int> &musicIds);
};

#endif // MUSICLIBRARYMODEL_H
