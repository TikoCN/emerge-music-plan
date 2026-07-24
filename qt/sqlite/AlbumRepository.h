#ifndef ALBUMREPOSITORY_H
#define ALBUMREPOSITORY_H

#include "Core.h"
#include "baseclass/Album.h"
#include "baseclass/MediaData.h"

class AlbumRepository {
private:
    Core *core;

public:
    explicit AlbumRepository(Core *core)
        : core(core) {
    }

    [[nodiscard]] QStringList getKeys() const;
    [[nodiscard]] QList<int>  getByKey(const QString &key, int size, int start) const;
    [[nodiscard]] AlbumPtr    get(int id) const;
    [[nodiscard]] QList<int>  getMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getMusicAll(int id, int sort) const;
    [[nodiscard]] int         getMusicFirst(int albumId) const;
    [[nodiscard]] QList<int>  getRandList() const;
    [[nodiscard]] QStringList getNameList(int size, int start) const;

    [[nodiscard]] bool append(const QStringList &albumList) const;
    [[nodiscard]] bool appendMusic(const QList<MediaData> &dataList) const;

    bool update(const AlbumPtr &album) const;
    bool updateMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const;
    bool updateNameKey(const QStringList &albumName, const QStringList &albumNameKey) const;

    bool moveMusic(const QString &albumName, const QString &albumNameNew) const;
    bool addMusicToPlayList(const QString &albumName, const QString &playListName) const;
};

#endif // ALBUMREPOSITORY_H
