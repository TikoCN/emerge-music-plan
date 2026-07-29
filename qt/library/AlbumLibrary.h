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

    Q_INVOKABLE void updateName(int albumId, const QString &name);
    Q_INVOKABLE void updateSort(int albumId, int sort);

    Q_INVOKABLE Album       getData(int id);
    Q_INVOKABLE QJsonObject getJson(int id);

    Q_INVOKABLE QStringList getKeys() const;

    Q_INVOKABLE bool moveMusic(const QString &albumName, const QString &albumNameNew) const;
    Q_INVOKABLE bool addMusicToPlayList(const QString &albumName, const QString &playListName) const;

private:
    explicit AlbumLibrary()  = default;
    ~AlbumLibrary() override = default;
};

#endif // ALBUMLIBRARY_H
