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

    Q_INVOKABLE void updateName(int artistId, const QString &name);
    Q_INVOKABLE void updateSort(int artistId, int sort);

    Q_INVOKABLE Artist      getData(int id);
    Q_INVOKABLE QJsonObject getJson(int id);

    Q_INVOKABLE QStringList getKeys() const;

    Q_INVOKABLE bool moveMusic(const QString &artistName, const QString &artistNameNew) const;
    Q_INVOKABLE bool addMusicToPlayList(const QString &artistName, const QString &playListName) const;

private:
    explicit ArtistLibrary()  = default;
    ~ArtistLibrary() override = default;
};

#endif // ARTISTLIBRARY_H
