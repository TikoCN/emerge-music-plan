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

    Q_INVOKABLE static QList<int> listSort(const QList<int> &musicIdList, SORT_TYPE sort);

    Q_INVOKABLE static void updateLove(int musicId, bool isLove);
    Q_INVOKABLE static void updateLevel(int musicId, bool level);

    Q_INVOKABLE static Music getData(int id);

    Q_INVOKABLE static QStringList getKeys();

private:
    explicit MusicLibrary()  = default;
    ~MusicLibrary() override = default;
};

#endif // MUSICLIBRARY_H
