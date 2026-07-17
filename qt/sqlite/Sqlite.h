#ifndef SQLITE_H
#define SQLITE_H
#include "Get.h"
#include "Append.h"
#include "Delete.h"
#include "Update.h"
#include "Core.h"

class SQLite : public QObject {
    Q_OBJECT

public:
    Get    getPort;
    Append appendPort;
    Delete deletePort;
    Update updatePort;

    static SQLite &getInstance() {
        static SQLite instance;
        return instance;
    }

    void begin() { core.begin(); }
    void commit() { core.commit(); }
    void rollback() { core.rollback(); }

    // 获得新的条目
    bool selectNewMusic(const QFileInfoList &infoList, QFileInfoList *newInfoList);

    // 清除不存在的条目
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

    Q_INVOKABLE [[nodiscard]] QStringList getArtistKeys() const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getArtistByKey(const QString &key, int size, int start) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getArtistMusic(int id, int size, int start, int sort) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getArtistRandList() const;
    Q_INVOKABLE [[nodiscard]] int         checkArtistName(const QString &name) const;

    Q_INVOKABLE [[nodiscard]] QStringList getAlbumKeys() const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getAlbumByKey(const QString &key, int size, int start) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getAlbumMusic(int id, int size, int start, int sort) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getAlbumRandList() const;
    Q_INVOKABLE [[nodiscard]] int         checkAlbumName(const QString &name) const;

    Q_INVOKABLE [[nodiscard]] QStringList getMusicKeys() const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getMusicByKey(const QString &key, int size, int start) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getMusicRandList(int length = -1) const;
    Q_INVOKABLE [[nodiscard]] QList<int>  getNewMusicList() const;

    Q_INVOKABLE [[nodiscard]] QString    getAllList() const;
    Q_INVOKABLE [[nodiscard]] QList<int> getReadMoreList() const;

    Q_INVOKABLE [[nodiscard]] QList<int> getPlayListMusic(int id, int size, int start, int sort) const;
    Q_INVOKABLE [[nodiscard]] QList<int> getPlayingListMusic() const;
    Q_INVOKABLE [[nodiscard]] int        checkPlayListName(const QString &name) const;

    Q_INVOKABLE bool updatePlayListMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    Q_INVOKABLE bool updateArtistMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const;
    Q_INVOKABLE bool updateAlbumMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const;

private:
    explicit SQLite();
    ~SQLite() override;

    Core core;
};

#endif // SQLITE_H
