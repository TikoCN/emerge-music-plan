#include "PlaylistLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"

void PlaylistLibrary::appendUser(const QString &name) {
    if (SQLite::getInstance().playlistRepository.appendUser(name)) {
    }
}

void PlaylistLibrary::updateName(const int playListId, const QString &name) {
    DataActive::getInstance().updatePlaylistName(playListId, name);
}

void PlaylistLibrary::updateSort(const int playListId, const int sort) {
    DataActive::getInstance().updatePlaylistSort(playListId, sort);
}

Playlist PlaylistLibrary::getData(const int id) {
    return DataActive::getInstance().getPlaylistData(id);
}

int PlaylistLibrary::allowName(const QString &name) {
    return SQLite::getInstance().playlistRepository.allowName(name);
}

bool PlaylistLibrary::moveMusic(const QString &playListName, const QString &playListNameNew) {
    return SQLite::getInstance().playlistRepository.moveMusic(playListName, playListNameNew);
}

bool PlaylistLibrary::addMusicToPlaylist(const QString &sourcePlaylistName, const QString &targetPlaylistName) {
    return SQLite::getInstance().playlistRepository.addMusicToPlaylist(sourcePlaylistName, targetPlaylistName);
}

bool PlaylistLibrary::addSingleMusicToPlaylist(const int musicId, const int playlistId) {
    return SQLite::getInstance().playlistRepository.addMusicToPlaylist(musicId, playlistId);
}

bool PlaylistLibrary::removeMusicFromPlaylist(const int musicId, const int playlistId) {
    return SQLite::getInstance().playlistRepository.removeMusicFromPlaylist(musicId, playlistId);
}

bool PlaylistLibrary::moveMusicToPlaylist(const int musicId, const int sourcePlaylistId, const int targetPlaylistId) {
    return SQLite::getInstance().playlistRepository.moveMusicToPlaylist(musicId, sourcePlaylistId, targetPlaylistId);
}
