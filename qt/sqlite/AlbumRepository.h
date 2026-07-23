#ifndef ALBUMREPOSITORY_H
#define ALBUMREPOSITORY_H

#include "Core.h"
#include "baseclass/Album.h"
#include "baseclass/MediaData.h"

class AlbumRepository {
private:
    Core *core;

public:
    explicit AlbumRepository(Core *core) : core(core) {}

    [[nodiscard]] QStringList getAlbumKeys() const;
    [[nodiscard]] QList<int> getAlbumByKey(const QString &key, int size, int start) const;
    [[nodiscard]] AlbumPtr getAlbum(int id) const;
    [[nodiscard]] QHash<int, AlbumPtr> getAlbum(const QList<int> &idList) const;
    [[nodiscard]] QList<int> getAlbumMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int> getAlbumMusicAll(int id, int sort) const;
    [[nodiscard]] int getAlbumMusicFirst(int albumId) const;
    [[nodiscard]] QList<int> getAlbumRandList() const;
    [[nodiscard]] int checkAlbumName(const QString &name) const;
    [[nodiscard]] QStringList getAlbumNameList(int size, int start) const;

    [[nodiscard]] bool appendAlbum(const QString &album) const;
    [[nodiscard]] bool appendAlbum(const QStringList &albumList) const;
    [[nodiscard]] bool appendAlbumMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendAlbumMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendAlbumMusic(const QList<QPair<QString, QString> > &pairList) const;
    [[nodiscard]] bool appendAlbumMusic(const QList<MediaData> &dataList) const;

    bool updateAlbum(const AlbumPtr &album) const;
    bool updateAlbumMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const;
    bool updateAlbumNameKey(const QStringList &albumName, const QStringList &albumNameKey) const;

    bool moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const;
    bool addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const;
};

#endif // ALBUMREPOSITORY_H