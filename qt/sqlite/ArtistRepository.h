#ifndef ARTISTREPOSITORY_H
#define ARTISTREPOSITORY_H

#include "Core.h"
#include "baseclass/Artist.h"
#include "baseclass/MediaData.h"

class ArtistRepository {
private:
    Core *core;

public:
    explicit ArtistRepository(Core *core)
        : core(core) {
    }

    [[nodiscard]] QStringList getKeys() const;
    [[nodiscard]] QList<int>  getByKey(const QString &key, int size, int start) const;
    [[nodiscard]] ArtistPtr   get(int id) const;
    [[nodiscard]] QList<int>  getMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getMusicAll(int id, int sort) const;
    [[nodiscard]] int         getMusicFirst(int artistId) const;
    [[nodiscard]] QList<int>  getRandList() const;
    [[nodiscard]] QStringList getNameList(int size, int start) const;

    [[nodiscard]] bool append(const QStringList &artistList) const;
    [[nodiscard]] bool appendMusic(const QList<MediaData> &dataList) const;

    [[nodiscard]] bool update(const ArtistPtr &artist) const;
    [[nodiscard]] bool updateMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const;
    [[nodiscard]] bool updateNameKey(const QStringList &artistName, const QStringList &artistNameKey) const;

    [[nodiscard]] bool moveMusic(const QString &artistName, const QString &artistNameNew) const;
    [[nodiscard]] bool addMusicToPlaylist(const QString &artistName, const QString &playListName) const;
};

#endif // ARTISTREPOSITORY_H
