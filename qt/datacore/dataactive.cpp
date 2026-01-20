#include "dataactive.h"
#include "sqlite/sqlite.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "sqlite/sqlite.h"
#include <algorithm>

void DataActive::appendPlayList(QString name)
{
    sql->appendUserPlayList(name);
}

QList<int> DataActive::playListShowAllMusic(int playListId)
{
    PlayListPtr playlist = getPlayListCore(playListId);
    QList<int> musicList = playlist->musicList;
    return musicList;
}

QList<int> DataActive::playListShowLoveMusic(int playListId)
{
    PlayListPtr playlist = getPlayListCore(playListId);
    QList<MusicPtr> musicList = getMusicCoreList(playlist->musicList);
    QList<int> newIdList;

    for (int i = 0; i < musicList.size(); ++i) {
        if (musicList[i]->isLove)
            newIdList.append(musicList[i]->id);
    }

    return newIdList;
}

QList<int> DataActive::playListShowSearchMusic(int playListId, QString e)
{
    PlayListPtr playlist = getPlayListCore(playListId);
    QList<MusicPtr> musicList = getMusicCoreList(playlist->musicList);
    QList<int> newIdList;

    for (int i = 0; i < musicList.size(); ++i) {
        if (musicList[i]->isSearch(e))
            newIdList.append(musicList[i]->id);
    }

    return newIdList;
}

QList<int> DataActive::musicListSort(QList<int> musicIdList, PlayList::SORT_TYPE sort)
{
    QList<MusicPtr> musicList = getMusicCoreList(musicIdList);
    QList<int> newIdList;

    switch (sort) {
    case PlayList::SORT_ALUMB_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->album > b->album;
        });
        break;
    case PlayList::SORT_ALUMB_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->album < b->album;
        });
        break;
    case PlayList::SORT_ATRIST_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->getArtist() > b->getArtist();
        });
        break;
    case PlayList::SORT_ATRIST_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->getArtist() < b->getArtist();
        });
        break;
    case PlayList::SORT_DURATION_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->duration > b->duration;
        });
        break;
    case PlayList::SORT_DURATION_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->duration < b->duration;
        });
        break;
    case PlayList::SORT_LAST_EDIT_TIME_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->lastEditTime > b->lastEditTime;
        });
        break;
    case PlayList::SORT_LAST_EDIT_TIME_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->lastEditTime < b->lastEditTime;
        });
        break;
    case PlayList::SORT_LEVEL_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->level > b->level;
        });
        break;
    case PlayList::SORT_LEVEL_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->level < b->level;
        });
        break;
    case PlayList::SORT_PLAY_NUMBER_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->playNumber > b->playNumber;
        });
        break;
    case PlayList::SORT_PLAY_NUMBER_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->playNumber < b->playNumber;
        });
        break;
    case PlayList::SORT_TITTLE_ASC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->title > b->title;
        });
        break;
    case PlayList::SORT_TITTLE_DESC:
        std::sort(musicList.begin(), musicList.end(), [](MusicPtr a, MusicPtr b)->bool{
            return a->title < b->title;
        });
        break;
    default:
        break;
    }

    for (int i = 0; i < musicList.size(); ++i) {
        newIdList.append(musicList[i]->id);
    }

    return newIdList;
}

void DataActive::clearNullItem()
{
    sql->clearNullMusicItem();
    sql->clearNullPlayListItem();
}

void DataActive::playlistAppendMusic(int id, QList<int> musicList)
{
    sql->appendPlayListMusic(id, musicList);

    PlayListPtr playlist = getPlayListCore(id);
    if (playlist != nullptr) {
        playlist->musicList.append(musicList);
    }
}

void DataActive::updateMusicLove(int musicId, bool isLove)
{
    MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        log->logError("");
        return;
    }

    music->isLove = isLove;
    sql->updateMusic(music);
}

void DataActive::updateMusicLevel(int musicId, bool level)
{
    MusicPtr music = getMusicCore(musicId);
    if (music.isNull()) {
        log->logError("");
        return;
    }

    music->level = level;
    sql->updateMusic(music);
}

void DataActive::updateAllNameKey()
{

}

DataActive::DataActive()
{

}

DataActive::~DataActive()
{

}
