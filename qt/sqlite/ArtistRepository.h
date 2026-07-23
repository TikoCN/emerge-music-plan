#ifndef ARTISTREPOSITORY_H
#define ARTISTREPOSITORY_H

#include "Core.h"
#include "baseclass/Artist.h"
#include "baseclass/MediaData.h"

class ArtistRepository {
private:
    Core *core;

public:
    explicit ArtistRepository(Core *core) : core(core) {}

    [[nodiscard]] QStringList getArtistKeys() const;
    [[nodiscard]] QList<int> getArtistByKey(const QString &key, int size, int start) const;
    [[nodiscard]] ArtistPtr getArtist(int id) const;
    [[nodiscard]] QHash<int, ArtistPtr> getArtist(const QList<int> &idList) const;
    [[nodiscard]] QList<int> getArtistMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int> getArtistMusicAll(int id, int sort) const;
    [[nodiscard]] int getArtistMusicFirst(int artistId) const;
    [[nodiscard]] QList<int> getArtistRandList() const;
    [[nodiscard]] int checkArtistName(const QString &name) const;
    [[nodiscard]] QStringList getArtistNameList(int size, int start) const;

    [[nodiscard]] bool appendArtist(const QString &artist) const;
    [[nodiscard]] bool appendArtist(const QStringList &artistList) const;
    [[nodiscard]] bool appendArtistMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendArtistMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendArtistMusic(const QList<QPair<QString, QString> > &pairList) const;
    [[nodiscard]] bool appendArtistMusic(const QList<MediaData> &dataList) const;

    bool updateArtist(const ArtistPtr &artist) const;
    bool updateArtistMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const;
    bool updateArtistNameKey(const QStringList &artistName, const QStringList &artistNameKey) const;

    bool moveArtistMusic(const QString &artistName, const QString &artistNameNew) const;
    bool addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const;
};

#endif // ARTISTREPOSITORY_H