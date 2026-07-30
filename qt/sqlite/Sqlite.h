#ifndef SQLITE_H
#define SQLITE_H
#include "Core.h"
#include "MusicRepository.h"
#include "AlbumRepository.h"
#include "ArtistRepository.h"
#include "PlaylistRepository.h"
#include "QueueRepository.h"
#include "baseclass/MediaData.h"

class SQLite : public QObject {
    Q_OBJECT

public:
    MusicRepository    musicRepository;
    AlbumRepository    albumRepository;
    QueueRepository    queueRepository;
    ArtistRepository   artistRepository;
    PlaylistRepository playlistRepository;

    static SQLite &getInstance() {
        static SQLite instance;
        return instance;
    }

    void begin() { core.begin(); }
    void commit() { core.commit(); }
    void rollback() { core.rollback(); }

    bool selectNewMusic(const QFileInfoList &infoList, QFileInfoList *newInfoList);

    QList<QString> clearNullMusicItem();
    QList<QString> clearNullPlaylistItem();

    void createTableMusic();

    void createTablePlaylist();
    void createTablePlaylistMusic();

    void createTableArtist();
    void createTableArtistMusic();

    void createTableAlbum();
    void createTableAlbumMusic();

    void createTableQueue();

    [[nodiscard]] bool insertMediaData(const QList<MediaData> &list) const;

private:
    explicit SQLite();
    ~SQLite() override;

    Core core;
};

#endif // SQLITE_H
