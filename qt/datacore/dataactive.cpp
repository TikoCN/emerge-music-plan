#include "dataactive.h"
#include "sqlite/sqlite.h"
#include <QDir>
#include <algorithm>
#include "namekey.h"

void DataActive::appendPlayList(const QString &name) const {
    sql->appendUserPlayList(name);
}

QList<int> DataActive::playListShowAllMusic(const int playListId)
{
    const PlayListPtr playlist = getPlayListCore(playListId);
    QList<int> musicList = playlist->musicList;
    return musicList;
}

QList<int> DataActive::playListShowLoveMusic(const int playListId)
{
    const PlayListPtr playlist = getPlayListCore(playListId);
    QList<MusicPtr> musicList = getMusicCoreList(playlist->musicList);
    QList<int> newIdList;

    for (const MusicPtr & i : musicList) {
        if (i->isLove)
            newIdList.append(i->id);
    }

    return newIdList;
}

QList<int> DataActive::playListShowSearchMusic(const int playListId, const QString& e)
{
    const PlayListPtr playlist = getPlayListCore(playListId);
    QList<MusicPtr> musicList = getMusicCoreList(playlist->musicList);
    QList<int> newIdList;

    for (const MusicPtr & i : musicList) {
        if (i->isSearch(e))
            newIdList.append(i->id);
    }

    return newIdList;
}

QList<int> DataActive::musicListSort(const QList<int> &musicIdList, PlayList::SORT_TYPE sort)
{
    QList<MusicPtr> musicList = getMusicCoreList(musicIdList);
    QList<int> newIdList;

    switch (sort) {
    case PlayList::SORT_ALBUM_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->album > b->album;
        });
        break;
    case PlayList::SORT_ALBUM_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->album < b->album;
        });
        break;
    case PlayList::SORT_ARTIST_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->getArtist() > b->getArtist();
        });
        break;
    case PlayList::SORT_ARTIST_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->getArtist() < b->getArtist();
        });
        break;
    case PlayList::SORT_DURATION_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->duration > b->duration;
        });
        break;
    case PlayList::SORT_DURATION_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->duration < b->duration;
        });
        break;
    case PlayList::SORT_LAST_EDIT_TIME_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->lastEditTime > b->lastEditTime;
        });
        break;
    case PlayList::SORT_LAST_EDIT_TIME_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->lastEditTime < b->lastEditTime;
        });
        break;
    case PlayList::SORT_LEVEL_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->level > b->level;
        });
        break;
    case PlayList::SORT_LEVEL_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->level < b->level;
        });
        break;
    case PlayList::SORT_PLAY_NUMBER_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr &b)->bool{
            return a->playNumber > b->playNumber;
        });
        break;
    case PlayList::SORT_PLAY_NUMBER_DESC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr& b)->bool{
            return a->playNumber < b->playNumber;
        });
        break;
    case PlayList::SORT_TITTLE_ASC:
        std::ranges::sort(musicList, [](const MusicPtr& a, const MusicPtr &b)->bool{
            return a->title > b->title;
        });
        break;
    case PlayList::SORT_TITTLE_DESC:
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
    sql->clearNullMusicItem();
    sql->clearNullPlayListItem();
}

void DataActive::playlistAppendMusic(const int id, const QList<int>& musicList)
{
    sql->appendPlayListMusic(id, musicList);

    const PlayListPtr playlist = getPlayListCore(id);
    if (playlist != nullptr) {
        playlist->musicList.append(musicList);
    }
}

void DataActive::updateMusicLove(const int musicId, const bool isLove)
{
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        log->logError("");
        return;
    }

    music->isLove = isLove;
    sql->updateMusic(music);
}

void DataActive::updateMusicLevel(const int musicId, const bool level)
{
    const MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        log->logError("");
        return;
    }

    music->level = level;
    sql->updateMusic(music);
}

void DataActive::updateALLNameKey() const {

    NameKey nameKey(log);

    int startPos = 0;
    constexpr int maxSize = 50;
    int resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = sql->getAlbumNameList(maxSize, startPos);
        QStringList nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos += resultSize;

        for (const QString &name : nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        sql->updateAlbumNameKey(nameList, nameKeyList);
    }

    startPos = 0;
    resultSize = maxSize;
    while (resultSize == maxSize) {
        const QStringList &nameList = sql->getArtistNameList(maxSize, startPos);
        QStringList nameKeyList;
        resultSize = static_cast<int>(nameList.size());
        startPos += resultSize;

        for (const QString &name : nameList) {
            const QString &key = nameKey.find(name);
            nameKeyList.append(key);
        }

        sql->updateArtistNameKey(nameList, nameKeyList);
    }
}

DataActive::DataActive()
= default;

DataActive::~DataActive()
= default;
