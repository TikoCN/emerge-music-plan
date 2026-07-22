#ifndef ALBUMLIBRARY_H
#define ALBUMLIBRARY_H

#include <QObject>
#include "baseclass/Album.h"

class AlbumLibraryModel;

class AlbumLibrary : public QObject {
    Q_OBJECT

public:
    static AlbumLibrary &getInstance() {
        static AlbumLibrary instance;
        return instance;
    }

    Q_INVOKABLE void updateAlbumName(int albumId, const QString &name);
    Q_INVOKABLE void updateAlbumSort(int albumId, int sort);

    Q_INVOKABLE Album getAlbumData(int id);
    Q_INVOKABLE QJsonObject getAlbumJson(int id);

    Q_INVOKABLE QStringList getAlbumKeys() const;

    Q_INVOKABLE int checkAlbumName(const QString &name) const;

    Q_INVOKABLE bool moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const;
    Q_INVOKABLE bool addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const;

    Q_INVOKABLE AlbumLibraryModel *model();

signals:
    void buildAlbumPlayer();
    void finish();

private:
    explicit AlbumLibrary();
    ~AlbumLibrary() override;

    AlbumLibraryModel *m_model;
};

#endif // ALBUMLIBRARY_H