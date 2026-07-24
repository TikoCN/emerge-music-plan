#ifndef ARTISTLIBRARY_H
#define ARTISTLIBRARY_H

#include <QObject>
#include "baseclass/Artist.h"
#include "datacore/DataLoader.h"

class ArtistLibraryModel;

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

    Q_INVOKABLE ArtistLibraryModel *model();

    Q_INVOKABLE DataLoader *loader();

    Q_INVOKABLE void loadByKey(const QString &key);
    Q_INVOKABLE void loadMoreByKey(int index, const QString &key);

signals:
    void buildPlayer();
    void finish();

private:
    explicit ArtistLibrary();
    ~ArtistLibrary() override;

    ArtistLibraryModel *m_model;
    DataLoader *        m_loader;
};

#endif // ARTISTLIBRARY_H
