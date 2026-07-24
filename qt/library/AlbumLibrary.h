#ifndef ALBUMLIBRARY_H
#define ALBUMLIBRARY_H

#include <QObject>
#include "baseclass/Album.h"
#include "datacore/DataLoader.h"

class AlbumLibraryModel;

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

    Q_INVOKABLE AlbumLibraryModel *model();

    Q_INVOKABLE DataLoader *loader();

    Q_INVOKABLE void loadByKey(const QString &key);
    Q_INVOKABLE void loadMoreByKey(int index, const QString &key);

signals:
    void buildPlayer();
    void finish();

private:
    explicit AlbumLibrary();
    ~AlbumLibrary() override;

    AlbumLibraryModel *m_model;
    DataLoader *       m_loader;
};

#endif // ALBUMLIBRARY_H
