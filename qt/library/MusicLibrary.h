#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include "baseclass/Music.h"
#include "datacore/DataLoader.h"

class MusicModel;

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    static MusicLibrary &getInstance() {
        static MusicLibrary instance;
        return instance;
    }

    Q_INVOKABLE QList<int> listSort(const QList<int> &musicIdList, SORT_TYPE sort);

    Q_INVOKABLE void updateLove(int musicId, bool isLove);
    Q_INVOKABLE void updateLevel(int musicId, bool level);

    Q_INVOKABLE Music       getMusicData(int id);
    Q_INVOKABLE QJsonObject getJson(int id);

    Q_INVOKABLE QStringList getKeys() const;

    Q_INVOKABLE void startClearInvalidData();

private:
    explicit MusicLibrary()  = default;
    ~MusicLibrary() override = default;
};

#endif // MUSICLIBRARY_H
