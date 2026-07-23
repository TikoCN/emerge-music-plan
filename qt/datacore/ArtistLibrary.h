#ifndef ARTISTLIBRARY_H
#define ARTISTLIBRARY_H

#include <QObject>
#include "baseclass/Artist.h"
#include "DataLoader.h"

class ArtistLibraryModel;

class ArtistLibrary : public QObject {
    Q_OBJECT

public:
    static ArtistLibrary &getInstance() {
        static ArtistLibrary instance;
        return instance;
    }

    Q_INVOKABLE void updateArtistName(int artistId, const QString &name);
    Q_INVOKABLE void updateArtistSort(int artistId, int sort);

    Q_INVOKABLE Artist getArtistData(int id);
    Q_INVOKABLE QJsonObject getArtistJson(int id);

    Q_INVOKABLE QStringList getArtistKeys() const;

    Q_INVOKABLE int checkArtistName(const QString &name) const;

    Q_INVOKABLE bool moveArtistMusic(const QString &artistName, const QString &artistNameNew) const;
    Q_INVOKABLE bool addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const;

    Q_INVOKABLE ArtistLibraryModel *model();

    Q_INVOKABLE DataLoader *loader();

    Q_INVOKABLE void loadByKey(const QString &key);
    Q_INVOKABLE void loadMoreByKey(int index, const QString &key);

signals:
    void buildArtistPlayer();
    void finish();

private:
    explicit ArtistLibrary();
    ~ArtistLibrary() override;

    ArtistLibraryModel *m_model;
    DataLoader *m_loader;
};

#endif // ARTISTLIBRARY_H