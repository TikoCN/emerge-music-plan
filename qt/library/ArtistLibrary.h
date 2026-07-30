#ifndef ARTISTLIBRARY_H
#define ARTISTLIBRARY_H

#include <QObject>
#include "baseclass/Artist.h"
#include "datacore/DataLoader.h"

class ArtistModel;

class ArtistLibrary : public QObject {
    Q_OBJECT

public:
    static ArtistLibrary &getInstance() {
        static ArtistLibrary instance;
        return instance;
    }

    Q_INVOKABLE static void updateName(int artistId, const QString &name);
    Q_INVOKABLE static void updateSort(int artistId, int sort);

    Q_INVOKABLE static Artist getData(int id);

    Q_INVOKABLE static QStringList getKeys();

    Q_INVOKABLE static bool moveMusic(const QString &artistName, const QString &artistNameNew);
    Q_INVOKABLE static bool addMusicToPlaylist(const QString &artistName, const QString &playListName);

private:
    explicit ArtistLibrary()  = default;
    ~ArtistLibrary() override = default;
};

#endif // ARTISTLIBRARY_H
