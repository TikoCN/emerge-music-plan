#ifndef MUSICREPOSITORY_H
#define MUSICREPOSITORY_H

#include "Core.h"
#include "baseclass/Music.h"
#include "baseclass/MediaData.h"

class MusicRepository {
private:
    Core *core;

public:
    explicit MusicRepository(Core *core) : core(core) {}

    [[nodiscard]] QStringList getMusicKeys() const;
    [[nodiscard]] QList<int> getMusicByKey(const QString &key, int size, int start) const;
    [[nodiscard]] MusicPtr getMusic(int id) const;
    [[nodiscard]] QHash<int, MusicPtr> getMusic(const QList<int> &idList) const;
    [[nodiscard]] QString getMusicUrl(int id) const;
    [[nodiscard]] QList<int> getMusicRandList(int length = -1) const;
    [[nodiscard]] QList<int> getNewMusicList() const;
    [[nodiscard]] QList<int> getReadMoreList() const;

    [[nodiscard]] bool appendMusic(const MediaData &data) const;
    [[nodiscard]] bool appendMusic(const QList<MediaData> &data) const;

    bool updateMusic(const MusicPtr &music) const;
    bool updateMusic(const QList<MusicPtr> &musicList) const;

    [[nodiscard]] QString getAllList() const;
    [[nodiscard]] QList<QPair<int, QString>> getAllMusicData() const;
    [[nodiscard]] bool clearInvalidData(const QList<int> &invalidMusicIds) const;
};

#endif // MUSICREPOSITORY_H