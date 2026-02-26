#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include <QDir>
#include <algorithm>
#include "namekey.h"

void DataActive::appendPlayList(const QString &name) const {
    m_sql->appendUserPlayList(name);
}

QList<int> DataActive::musicListSort(const QList<int> &musicIdList, SORT_TYPE sort)
{
    QList<MusicPtr> musicList = getMusicCoreList(musicIdList);
    QList<int> newIdList;

    switch (sort) {
    case SORT_ALBUM_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->album > b->album;
        });
        break;
    case SORT_ALBUM_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->album < b->album;
        });
        break;
    case SORT_ARTIST_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->artist > b->artist;
        });
        break;
    case SORT_ARTIST_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->artist < b->artist;
        });
        break;
    case SORT_DURATION_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->duration > b->duration;
        });
        break;
    case SORT_DURATION_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->duration < b->duration;
        });
        break;
    case SORT_LAST_EDIT_TIME_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->lastEditTime > b->lastEditTime;
        });
        break;
    case SORT_LAST_EDIT_TIME_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->lastEditTime < b->lastEditTime;
        });
        break;
    case SORT_LEVEL_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->level > b->level;
        });
        break;
    case SORT_LEVEL_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->level < b->level;
        });
        break;
    case SORT_PLAY_NUMBER_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr &b)->bool{
            return a->playNumber > b->playNumber;
        });
        break;
    case SORT_PLAY_NUMBER_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->playNumber < b->playNumber;
        });
        break;
    case SORT_TITTLE_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr &b)->bool{
            return a->title > b->title;
        });
        break;
    case SORT_TITTLE_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr &b)->bool{
            return a->title < b->title;
        });
        break;
    default:
        break;
    }

    for (const MusicPtr & i : musicList) {
        newIdList.append(i->id);
    }

    return newIdList;
}

void DataActive::clearNullItem() const {
    m_sql->clearNullMusicItem();
    m_sql->clearNullPlayListItem();
}

void DataActive::updateMusicLove(const int musicId, const bool isLove)
{
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        m_loger->logError("更新歌曲喜爱失败");
        return;
    }

    music->isLove = isLove;
    m_sql->updateMusic(music);
}

void DataActive::updateMusicLevel(const int musicId, const bool level)
{
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        m_loger->logError("更新歌曲评级失败");
        return;
    }

    music->level = level;
    m_sql->updateMusic(music);
}

void DataActive::updatePlayListName(const int playListId, const QString &name) {
    const PlayListPtr playList = getPlayListCore(playListId);
    if (playList.isNull()) {
        m_loger->logError("更新列表姓名失败");
        return;
    }

    playList->name = name;
    m_sql->updatePlayList(playList);
}

void DataActive::updatePlayListSort(int playListId, int sort) {
    const PlayListPtr playList = getPlayListCore(playListId);
    if (playList.isNull()) {
        m_loger->logError("更新列表排序失败");
        return;
    }

    playList->sortType = static_cast<SORT_TYPE>(sort);
    m_sql->updatePlayList(playList);
}

void DataActive::updateArtistName(const int artistId, const QString &name) {
    const ArtistPtr artist = getArtistCore(artistId);
    if (artist.isNull()) {
        m_loger->logError("更新歌手名称失败");
        return;
    }

    artist->name = name;
    m_sql->updateArtist(artist);
}

void DataActive::updateArtistSort(int artistId, int sort) {
    const ArtistPtr artist = getArtistCore(artistId);
    if (artist.isNull()) {
        m_loger->logError("更新歌手排序失败");
        return;
    }

    artist->sortType = static_cast<SORT_TYPE>(sort);
    m_sql->updateArtist(artist);
}

void DataActive::updateAlbumName(const int albumId, const QString &name) {
    const AlbumPtr album = getAlbumCore(albumId);
    if (album.isNull()) {
        m_loger->logError("更新专辑名称失败");
        return;
    }

    album->name = name;
    m_sql->updateAlbum(album);
}

void DataActive::updateAlbumSort(const int albumId, int sort) {
    const AlbumPtr album = getAlbumCore(albumId);
    if (album.isNull()) {
        m_loger->logError("更新专辑排序失败");
        return;
    }

    album->sortType = static_cast<SORT_TYPE>(sort);
    m_sql->updateAlbum(album);
}

void DataActive::updateALLNameKey() const {

    NameKey nameKey(m_loger);

    int startPos = 0;
    constexpr int maxSize = 50;
    int resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = m_sql->getAlbumNameList(maxSize, startPos);
        QStringList nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos += resultSize;

        for (const QString &name : nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        m_sql->updateAlbumNameKey(nameList, nameKeyList);
    }

    startPos = 0;
    resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = m_sql->getArtistNameList(maxSize, startPos);
        QStringList nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos += resultSize;

        for (const QString &name : nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        m_sql->updateArtistNameKey(nameList, nameKeyList);
    }
}

DataActive::DataActive()
= default;

DataActive::~DataActive()
= default;
