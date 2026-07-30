#ifndef MUSICREPOSITORY_H
#define MUSICREPOSITORY_H

#include "Core.h"
#include "baseclass/Music.h"
#include "baseclass/MediaData.h"

class MusicRepository {
private:
    Core *core;

public:
    explicit MusicRepository(Core *core)
        : core(core) {
    }

    [[nodiscard]] QStringList          getKeys() const;
    [[nodiscard]] QList<int>           getByKey(const QString &key, int size, int start) const;
    [[nodiscard]] MusicPtr             get(int id) const;
    [[nodiscard]] QHash<int, MusicPtr> get(const QList<int> &idList) const;
    [[nodiscard]] QString              getUrl(int id) const;
    [[nodiscard]] QList<int>           getRandList(int length = -1) const;
    [[nodiscard]] QList<int>           getMostNew() const;
    [[nodiscard]] QList<int>           getReadMore() const;
    [[nodiscard]] QList<int>           getMostPlayed() const;

    [[nodiscard]] bool append(const QList<MediaData> &data) const;

    [[nodiscard]] bool update(const MusicPtr &music) const;

    [[nodiscard]] QList<QPair<int, QString> > getAllData() const;
    [[nodiscard]] bool                        clearInvalidData(const QList<int> &invalidMusicIds) const;
};

#endif // MUSICREPOSITORY_H
