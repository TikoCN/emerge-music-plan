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

    [[nodiscard]] QStringList getArtistKeys() const;
    [[nodiscard]] QList<int>  getArtistByKey(const QString &key, int size, int start) const;
    [[nodiscard]] QList<int>  getArtistMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getArtistRandList() const;
    [[nodiscard]] int         checkArtistName(const QString &name) const;

    [[nodiscard]] QStringList getAlbumKeys() const;
    [[nodiscard]] QList<int>  getAlbumByKey(const QString &key, int size, int start) const;
    [[nodiscard]] QList<int>  getAlbumMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getAlbumRandList() const;
    [[nodiscard]] int         checkAlbumName(const QString &name) const;

    [[nodiscard]] QStringList getMusicKeys() const;
    [[nodiscard]] QList<int>  getMusicByKey(const QString &key, int size, int start) const;
    [[nodiscard]] QList<int>  getMusicRandList(int length = -1) const;
    [[nodiscard]] QList<int>  getNewMusicList() const;

    [[nodiscard]] QString    getAllList() const;
    [[nodiscard]] QList<int> getReadMoreList() const;

    [[nodiscard]] QList<int> getPlayListMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int> getPlayingListMusic() const;
    [[nodiscard]] int        checkPlayListName(const QString &name) const;

    bool updatePlayListMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    bool updateArtistMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const;
    bool updateAlbumMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const;

    bool moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const;
    bool moveArtistMusic(const QString &artistName, const QString &artistNameNew) const;
    bool movePlayListMusic(const QString &playListName, const QString &playListNameNew) const;

    bool addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const;
    bool addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const;
    bool addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const;

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
