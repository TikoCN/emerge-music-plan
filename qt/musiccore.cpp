#include "musiccore.h"
#include "sqlite/sqlite.h"
#include "tlog.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "musiccore.h"
#include "sqlite/sqlite.h"
#include "tlog.h"
#include <algorithm>

void MusicCore::appendPlayList(QString name)
{
    SQLite::getInstance()->appendUserPlayList(name);
}

AlbumPtr MusicCore::getAlbumCore(int id)
{
    m_albumMutex.lock();

    TLog::getInstance()->logInfo(QString("获得 Album %1").arg(id));

    AlbumPtr album = nullptr;
    if (m_albumHash.contains(id)) {
        album = m_albumHash.value(id);
    } else {
        SQLite *sql = SQLite::getInstance();
        album = sql->getAlbum(id);
        if (album != nullptr) {
            m_albumHash.insert(id, album);
        }
    }

    m_albumMutex.unlock();
    deleteOutChace(MusicCore::CORE_TYPE::ALBUM, id);
    return album;
}

QJsonObject MusicCore::getAlbumJson(int id)
{
    AlbumPtr album = getAlbumCore(id);
    QJsonObject json = album->getJsonObject();

    return json;
}

QString MusicCore::getAlbumName(int id)
{
    if (m_albumHash.contains(id))
        return m_albumHash.value(id)->name;
    return "";
}

ArtistPtr MusicCore::getArtistCore(int id)
{
    m_artistMutex.lock();
    TLog::getInstance()->logInfo(QString("获得 Artist %1").arg(id));

    ArtistPtr artist = nullptr;
    if (m_artistHash.contains(id)) {
        artist = m_artistHash.value(id);
    } else {
        SQLite *sql = SQLite::getInstance();
        artist = sql->getArtist(id);
        if (artist != nullptr) {
            m_artistHash.insert(id, artist);
        }
    }

    m_artistMutex.unlock();
    deleteOutChace(MusicCore::CORE_TYPE::ARTIST, id);
    return artist;
}

QJsonObject MusicCore::getArtistJson(int id)
{
    ArtistPtr artist = getArtistCore(id);
    QJsonObject json = artist->getJsonObject();

    return json;
}

QString MusicCore::getArtistName(int id)
{
    if (m_artistHash.contains(id))
        return m_artistHash.value(id)->name;
    return "";
}

MusicPtr MusicCore::getMusicCore(int id)
{
    m_musicMutex.lock();
    TLog::getInstance()->logInfo(QString("获得 Music %1").arg(id));

    MusicPtr music = nullptr;
    if (m_musicHash.contains(id)) {
        music = m_musicHash.value(id);
    } else {
        SQLite *sql = SQLite::getInstance();
        music = sql->getMusic(id);
        if (music != nullptr) {
            m_musicHash.insert(id, music);
        }
    }

    m_musicMutex.unlock();
    deleteOutChace(MusicCore::CORE_TYPE::MUSIC, id);
    return music;
}

QList<MusicPtr> MusicCore::getMusicCoreList(QList<int> idList)
{
    TLog::getInstance()->logInfo(QString("获得 Music %1").arg(idList.size()));

    QList<MusicPtr> musicList;
    QList<int> newIdList;
    for (int i = 0; i < idList.size(); ++i) {
        if (!m_musicHash.contains(idList[i]))
            newIdList.append(idList[i]);
        else
            musicList.append(m_musicHash.value(idList[i]));
    }
    QHash<int, MusicPtr> hash = SQLite::getInstance()->getMusic(newIdList);
    m_musicHash.insert(hash);

    musicList.append(hash.values());
    return musicList;
}

QJsonObject MusicCore::getMusicJson(int id)
{
    MusicPtr music = getMusicCore(id);
    QJsonObject json = music->getJsonObject();
    return json;
}

QList<int> MusicCore::playListShowAllMusic(int playListId)
{
    PlayListPtr playlist = getPlayListCore(playListId);
    QList<int> musicList = playlist->musicList;
    return musicList;
}

QList<int> MusicCore::playListShowLoveMusic(int playListId)
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

QList<int> MusicCore::playListShowSearchMusic(int playListId, QString e)
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

QList<int> MusicCore::playListShowSortMusic(int playListId, PlayList::SORT_TYPE sort)
{
    PlayListPtr playlist = getPlayListCore(playListId);
    QList<MusicPtr> musicList = getMusicCoreList(playlist->musicList);
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

PlayListPtr MusicCore::getPlayListCore(int id)
{
    m_playlistMutex.lock();
    TLog::getInstance()->logInfo(QString("获得 PlayList %1").arg(id));

    PlayListPtr playlist = nullptr;
    if (m_playlistHash.contains(id)) {
        playlist = m_playlistHash.value(id);
    } else {
        SQLite *sql = SQLite::getInstance();
        playlist = sql->getList(id);
        if (playlist != nullptr) {
            m_playlistHash.insert(id, playlist);
        }
    }

    m_playlistMutex.unlock();
    deleteOutChace(MusicCore::CORE_TYPE::PLAYLIST, id);
    return playlist;
}

QJsonObject MusicCore::getPlayListJson(int id)
{
    PlayListPtr playlist = getPlayListCore(id);
    QJsonObject json = playlist->getJsonObject();
    return json;
}

void MusicCore::clearNullItem()
{
    SQLite::getInstance()->clearNullMusicItem();
    SQLite::getInstance()->clearNullPlayListItem();
}

void MusicCore::loadMusic(QList<int> idList)
{
    QList<int> newIdList;
    for (int i = 0; i < idList.size(); ++i) {
        if (!m_musicHash.contains(idList[i]))
            newIdList.append(idList[i]);
    }
    QHash<int, MusicPtr> hash = SQLite::getInstance()->getMusic(newIdList);
    m_musicHash.insert(hash);
}

void MusicCore::releaseAlbum(int id)
{
    m_albumMutex.lock();
    TLog::getInstance()->logError(tr("执行释放") +
                                  tr("专辑ID:%1").arg(id));
    m_albumHash.remove(id);
    m_albumMutex.unlock();
}

void MusicCore::releaseArtist(int id)
{
    m_artistMutex.lock();
    TLog::getInstance()->logError(tr("执行释放") +
                                  tr("歌手ID:%1").arg(id));
    m_artistHash.remove(id);
    m_artistMutex.unlock();
}

void MusicCore::releaseMusic(int id)
{
    m_musicMutex.lock();
    TLog::getInstance()->logError(tr("执行释放") +
                                  tr("歌曲ID:%1").arg(id));
    m_musicHash.remove(id);
    m_musicMutex.unlock();
}

void MusicCore::releasePlayList(int id)
{
    m_playlistMutex.lock();
    TLog::getInstance()->logError(tr("执行释放") +
                                  tr("播放列表ID:%1").arg(id));
    m_playlistHash.remove(id);
    m_playlistMutex.unlock();
}

MusicCore::~MusicCore()
{
    m_albumHash.clear();
    m_artistHash.clear();
    m_musicHash.clear();
    m_playlistHash.clear();
    m_deleteList.clear();
}

void MusicCore::playlistAppendMusic(int id, QList<int> musicList)
{
    SQLite::getInstance()->appendPlayListMusic(id, musicList);

    PlayListPtr playlist = getPlayListCore(id);
    if (playlist != nullptr) {
        playlist->musicList.append(musicList);
    }
}

MusicCore::MusicCore()
{
}

void MusicCore::deleteOutChace(MusicCore::CORE_TYPE type, int id)
{
    m_deleteListMutex.lock();

    //插入
    QPair<MusicCore::CORE_TYPE, int> pair(type, id);
    int pos = m_deleteList.indexOf(pair);
    if (pos != -1) {
        m_deleteList.move(pos, m_deleteList.size() - 1);
    } else {
        m_deleteList.append(pair);
    }

    //删除
    if (m_deleteList.size() > 50) {
        QPair<MusicCore::CORE_TYPE, int> deleteCore = m_deleteList.takeFirst();
        m_deleteListMutex.unlock();

        switch (deleteCore.first) {
        case MusicCore::CORE_TYPE::ALBUM:
            releaseAlbum(deleteCore.second);
            break;
        case MusicCore::CORE_TYPE::ARTIST:
            releaseArtist(deleteCore.second);
            break;
        case MusicCore::CORE_TYPE::MUSIC:
            releaseMusic(deleteCore.second);
            break;
        case MusicCore::CORE_TYPE::PLAYLIST:
            releasePlayList(deleteCore.second);
            break;
        default:
            break;
        }
    } else {
        m_deleteListMutex.unlock();
    }
}
