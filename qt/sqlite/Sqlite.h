#ifndef SQLITE_H
#define SQLITE_H
#include "Core.h"
#include "MusicRepository.h"
#include "AlbumRepository.h"
#include "ArtistRepository.h"
#include "PlayListRepository.h"
#include "baseclass/MediaData.h"

class SQLite : public QObject {
    Q_OBJECT

public:
    MusicRepository    musicRepository;
    AlbumRepository    albumRepository;
    ArtistRepository   artistRepository;
    PlayListRepository playListRepository;

    static SQLite &getInstance() {
        static SQLite instance;
        return instance;
    }

    void begin() { core.begin(); }
    void commit() { core.commit(); }
    void rollback() { core.rollback(); }

    bool selectNewMusic(const QFileInfoList &infoList, QFileInfoList *newInfoList);

    QList<QString> clearNullMusicItem();
    QList<QString> clearNullPlayListItem();

    void createTableMusic();
    void createTablePlayinglist();

    void createTablePlayList();
    void createTablePlayListMusic();

    void createTableArtist();
    void createTableArtistMusic();

    void createTableAlbum();
    void createTableAlbumMusic();



    void startClearInvalidData();
    void onCheckFileFinished(const QList<int> &invalidMusicIds);

    bool insertMediaData(const QList<MediaData> &list);

private:
    explicit SQLite();
    ~SQLite() override;

    Core core;

signals:
    void startCheckFileExist(const QList<QPair<int, QString> > &musicDataList);
    void invalidDataCleared();
};

#endif // SQLITE_H
