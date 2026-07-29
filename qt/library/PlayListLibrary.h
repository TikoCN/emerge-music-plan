#ifndef PLAYLISTLIBRARY_H
#define PLAYLISTLIBRARY_H

#include <QObject>
#include "baseclass/Playlist.h"
#include "datacore/DataLoader.h"

class PlaylistModel;

class PlayListLibrary : public QObject {
    Q_OBJECT

public:
    static PlayListLibrary &getInstance() {
        static PlayListLibrary instance;
        return instance;
    }

    Q_INVOKABLE void appendUser(const QString &name);

    Q_INVOKABLE void updateName(int playListId, const QString &name);
    Q_INVOKABLE void updateSort(int playListId, int sort);

    Q_INVOKABLE PlayList    getData(int id);
    Q_INVOKABLE QJsonObject getJson(int id);

    Q_INVOKABLE int allowName(const QString &name) const;

    Q_INVOKABLE bool moveMusic(const QString &playListName, const QString &playListNameNew) const;
    Q_INVOKABLE bool addMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const;

    Q_INVOKABLE void clearNullItem();
    Q_INVOKABLE void updateALLNameKey() const;

private:
    explicit PlayListLibrary()  = default;
    ~PlayListLibrary() override = default;
};

#endif // PLAYLISTLIBRARY_H
