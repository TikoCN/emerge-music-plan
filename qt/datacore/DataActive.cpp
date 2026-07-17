#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include <QDir>
#include <algorithm>
#include "namekey.h"

void DataActive::appendPlayList(const QString &name) {
    if (!SQLite::getInstance().appendPort.appendUserPlayList(name))
        TLog::getInstance().logError("插入播放列表失败");
}

QList<int> DataActive::musicListSort(const QList<int> &musicIdList, const SORT_TYPE sort) {
    QList<MusicPtr> musicList = getMusicCoreList(musicIdList);
    QList<int>      newIdList;

    switch (sort) {
        case SORT_ALBUM_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->album > b->album;
            });
            break;
        case SORT_ALBUM_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->album < b->album;
            });
            break;
        case SORT_ARTIST_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->artist > b->artist;
            });
            break;
        case SORT_ARTIST_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->artist < b->artist;
            });
            break;
        case SORT_DURATION_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->duration > b->duration;
            });
            break;
        case SORT_DURATION_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->duration < b->duration;
            });
            break;
        case SORT_LAST_EDIT_TIME_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->lastEditTime > b->lastEditTime;
            });
            break;
        case SORT_LAST_EDIT_TIME_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->lastEditTime < b->lastEditTime;
            });
            break;
        case SORT_LEVEL_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->level > b->level;
            });
            break;
        case SORT_LEVEL_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->level < b->level;
            });
            break;
        case SORT_PLAY_NUMBER_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->playNumber > b->playNumber;
            });
            break;
        case SORT_PLAY_NUMBER_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->playNumber < b->playNumber;
            });
            break;
        case SORT_TITTLE_ASC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->title > b->title;
            });
            break;
        case SORT_TITTLE_DESC:
            std::ranges::sort(musicList, [](const MusicPtr &a, const MusicPtr &b)-> bool {
                return a->title < b->title;
            });
            break;
        default:
            break;
    }

    for (const MusicPtr &i: musicList) {
        newIdList.append(i->id);
    }

    return newIdList;
}

void DataActive::clearNullItem() {
    SQLite::getInstance().clearNullMusicItem();
    SQLite::getInstance().clearNullPlayListItem();
}

void DataActive::updateMusicLove(const int musicId, const bool isLove) {
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        TLog::getInstance().logError("更新歌曲喜爱失败");
        return;
    }

    music->isLove = isLove;
    SQLite::getInstance().updatePort.updateMusic(music);
}

void DataActive::updateMusicLevel(const int musicId, const bool level) {
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        TLog::getInstance().logError("更新歌曲评级失败");
        return;
    }

    music->level = level;
    SQLite::getInstance().updatePort.updateMusic(music);
}

void DataActive::updatePlayListName(const int playListId, const QString &name) {
    const PlayListPtr playList = getPlayListCore(playListId);
    if (playList.isNull()) {
        TLog::getInstance().logError("更新列表姓名失败");
        return;
    }

    playList->name = name;
    SQLite::getInstance().updatePort.updatePlayList(playList);
}

void DataActive::updatePlayListSort(int playListId, int sort) {
    const PlayListPtr playList = getPlayListCore(playListId);
    if (playList.isNull()) {
        TLog::getInstance().logError("更新列表排序失败");
        return;
    }

    playList->sort = static_cast<SORT_TYPE>(sort);
    SQLite::getInstance().updatePort.updatePlayList(playList);
}

void DataActive::updateArtistName(const int artistId, const QString &name) {
    const ArtistPtr artist = getArtistCore(artistId);
    if (artist.isNull()) {
        TLog::getInstance().logError("更新歌手名称失败");
        return;
    }

    artist->name = name;
    SQLite::getInstance().updatePort.updateArtist(artist);
}

void DataActive::updateArtistSort(const int artistId, const int sort) {
    const ArtistPtr artist = getArtistCore(artistId);
    if (artist.isNull()) {
        TLog::getInstance().logError("更新歌手排序失败");
        return;
    }

    artist->sort = sort;
    SQLite::getInstance().updatePort.updateArtist(artist);
}

void DataActive::updateAlbumName(const int albumId, const QString &name) {
    const AlbumPtr album = getAlbumCore(albumId);
    if (album.isNull()) {
        TLog::getInstance().logError("更新专辑名称失败");
        return;
    }

    album->name = name;
    SQLite::getInstance().updatePort.updateAlbum(album);
}

void DataActive::updateAlbumSort(const int albumId, int sort) {
    const AlbumPtr album = getAlbumCore(albumId);
    if (album.isNull()) {
        TLog::getInstance().logError("更新专辑排序失败");
        return;
    }

    album->sort = static_cast<SORT_TYPE>(sort);
    SQLite::getInstance().updatePort.updateAlbum(album);
}

void DataActive::updateALLNameKey() const {
    NameKey nameKey;

    int           startPos   = 0;
    constexpr int maxSize    = 50;
    int           resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = SQLite::getInstance().getPort.getAlbumNameList(maxSize, startPos);
        QStringList        nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos   += resultSize;

        for (const QString &name: nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        SQLite::getInstance().updatePort.updateAlbumNameKey(nameList, nameKeyList);
    }

    startPos   = 0;
    resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = SQLite::getInstance().getPort.getArtistNameList(maxSize, startPos);
        QStringList        nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos   += resultSize;

        for (const QString &name: nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        SQLite::getInstance().updatePort.updateArtistNameKey(nameList, nameKeyList);
    }
}

DataActive::DataActive()
= default;

DataActive::~DataActive()
= default;
