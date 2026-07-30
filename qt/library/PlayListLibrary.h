#ifndef PLAYLISTLIBRARY_H
#define PLAYLISTLIBRARY_H

#include <QObject>
#include "baseclass/Playlist.h"
#include "datacore/DataLoader.h"

class PlaylistModel;

class PlaylistLibrary : public QObject {
    Q_OBJECT

public:
    static PlaylistLibrary &getInstance() {
        static PlaylistLibrary instance;
        return instance;
    }

    Q_INVOKABLE static void appendUser(const QString &name);

    Q_INVOKABLE static void updateName(int playListId, const QString &name);
    Q_INVOKABLE static void updateSort(int playListId, int sort);

    Q_INVOKABLE static Playlist getData(int id);

    Q_INVOKABLE static int allowName(const QString &name);

    Q_INVOKABLE static bool moveMusic(const QString &playListName, const QString &playListNameNew);
    Q_INVOKABLE static bool addMusicToPlaylist(const QString &sourcePlaylistName, const QString &targetPlaylistName);

private:
    explicit PlaylistLibrary()  = default;
    ~PlaylistLibrary() override = default;
};

#endif // PLAYLISTLIBRARY_H
