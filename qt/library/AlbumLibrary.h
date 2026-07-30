#ifndef ALBUMLIBRARY_H
#define ALBUMLIBRARY_H

#include <QObject>
#include "baseclass/Album.h"
#include "datacore/DataLoader.h"

class AlbumModel;

class AlbumLibrary : public QObject {
    Q_OBJECT

public:
    static AlbumLibrary &getInstance() {
        static AlbumLibrary instance;
        return instance;
    }

    Q_INVOKABLE static void updateName(int albumId, const QString &name);
    Q_INVOKABLE static void updateSort(int albumId, int sort);

    Q_INVOKABLE static Album getData(int id);

    Q_INVOKABLE static QStringList getKeys();

    Q_INVOKABLE static bool moveMusic(const QString &albumName, const QString &albumNameNew);
    Q_INVOKABLE static bool addMusicToPlaylist(const QString &albumName, const QString &playListName);

private:
    explicit AlbumLibrary()  = default;
    ~AlbumLibrary() override = default;
};

#endif // ALBUMLIBRARY_H
