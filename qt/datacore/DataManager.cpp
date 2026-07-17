#include "DataManager.h"

#include "sqlite/Sqlite.h"
#include "Tlog.h"
#include <QDir>

DataManager::DataManager() {
}

AlbumPtr DataManager::getAlbumCore(const int id) {
    m_albumMutex.lock();

    AlbumPtr album = nullptr;
    if (m_albumHash.contains(id)) {
        album = m_albumHash.value(id);
    } else {
        album = SQLite::getInstance().getPort.getAlbum(id);

        if (album != nullptr) {
            m_albumHash.insert(id, album);
        } else {
            TLog::getInstance().logError(QString("获取 %1 AlbumCorePtr失败").arg(id));
        }
    }

    m_albumMutex.unlock();
    deleteOutCache(ALBUM, id);
    return album;
}

QJsonObject DataManager::getAlbumJson(const int id) {
    const AlbumPtr album = getAlbumCore(id);
    QJsonObject    json  = album->getJsonObject();

    return json;
}

Album DataManager::getAlbumData(const int id) {
    const AlbumPtr album = getAlbumCore(id);
    if (album != nullptr) {
        return *album;
    }

    return {};
}

Music DataManager::getMusicData(const int id) {
    const MusicPtr music = getMusicCore(id);
    if (music != nullptr) {
        return *music;
    }

    return {};
}

PlayList DataManager::getPlayListData(const int id) {
    const PlayListPtr playList = getPlayListCore(id);
    if (playList != nullptr) {
        return *playList;
    }

    return {};
}

ArtistPtr DataManager::getArtistCore(const int id) {
    m_artistMutex.lock();

    ArtistPtr artist = nullptr;
    if (m_artistHash.contains(id)) {
        artist = m_artistHash.value(id);
    } else {
        artist = SQLite::getInstance().getPort.getArtist(id);
        if (artist != nullptr) {
            m_artistHash.insert(id, artist);
        } else {
            TLog::getInstance().logError(QString("获取 %1 ArtistCorePtr失败").arg(id));
        }
    }

    m_artistMutex.unlock();
    deleteOutCache(ARTIST, id);
    return artist;
}

QJsonObject DataManager::getArtistJson(const int id) {
    const ArtistPtr artist = getArtistCore(id);
    QJsonObject     json   = artist->getJsonObject();

    return json;
}

Artist DataManager::getArtistData(const int id) {
    const ArtistPtr artist = getArtistCore(id);
    if (artist != nullptr) {
        return *artist;
    }

    return {};
}

MusicPtr DataManager::getMusicCore(const int id) {
    m_musicMutex.lock();

    MusicPtr music = nullptr;
    if (m_musicHash.contains(id)) {
        music = m_musicHash.value(id);
    } else {
        music = SQLite::getInstance().getPort.getMusic(id);
        if (music != nullptr) {
            m_musicHash.insert(id, music);
        } else {
            TLog::getInstance().logError(QString("获取 %1 MusicCorePtr失败").arg(id));
        }
    }

    m_musicMutex.unlock();
    deleteOutCache(MUSIC, id);
    return music;
}

QList<MusicPtr> DataManager::getMusicCoreList(const QList<int> &idList) {
    TLog::getInstance().logInfo(QString("获得 Music %1").arg(idList.size()));

    QList<MusicPtr> musicList;
    QList<int>      newIdList;
    for (int i: idList) {
        if (!m_musicHash.contains(i))
            newIdList.append(i);
        else
            musicList.append(m_musicHash.value(i));
    }
    const QHash<int, MusicPtr> hash = SQLite::getInstance().getPort.getMusic(newIdList);
    m_musicHash.insert(hash);

    musicList.append(hash.values());
    return musicList;
}

QJsonObject DataManager::getMusicJson(const int id) {
    const MusicPtr music = getMusicCore(id);
    if (music == nullptr) {
        TLog::getInstance().logError("获取MusicCore失败");
        return {};
    };

    QJsonObject json = music->getJsonObject();
    return json;
}

PlayListPtr DataManager::getPlayListCore(const int id) {
    m_playlistMutex.lock();

    PlayListPtr playlist = nullptr;
    if (m_playlistHash.contains(id)) {
        playlist = m_playlistHash.value(id);
    } else {
        playlist = SQLite::getInstance().getPort.getList(id);
        if (playlist != nullptr) {
            m_playlistHash.insert(id, playlist);
        } else {
            TLog::getInstance().logError(QString("获取 %1 PlayCorePtr失败").arg(id));
        }
    }

    m_playlistMutex.unlock();
    deleteOutCache(PLAYLIST, id);
    return playlist;
}

QJsonObject DataManager::getPlayListJson(const int id) {
    const PlayListPtr playlist = getPlayListCore(id);
    QJsonObject       json     = playlist->getJsonObject();
    return json;
}

void DataManager::releaseAlbum(const int id) {
    m_albumMutex.lock();
    TLog::getInstance().logError(tr("执行释放") +
                                 tr("专辑ID:%1").arg(id));
    m_albumHash.remove(id);
    m_albumMutex.unlock();
}

void DataManager::releaseArtist(const int id) {
    m_artistMutex.lock();
    TLog::getInstance().logInfo(tr("执行释放") + tr("歌手ID:%1").arg(id));
    m_artistHash.remove(id);
    m_artistMutex.unlock();
}

void DataManager::releaseMusic(const int id) {
    m_musicMutex.lock();
    TLog::getInstance().logInfo(tr("执行释放") + tr("歌曲ID:%1").arg(id));
    m_musicHash.remove(id);
    m_musicMutex.unlock();
}

void DataManager::releasePlayList(const int id) {
    m_playlistMutex.lock();
    TLog::getInstance().logInfo(tr("执行释放") + tr("播放列表ID:%1").arg(id));
    m_playlistHash.remove(id);
    m_playlistMutex.unlock();
}

void DataManager::deleteOutCache(CORE_TYPE type, int id) {
    m_deleteListMutex.lock();

    //插入
    const QPair pair(type, id);
    if (const long long pos = m_deleteList.indexOf(pair); pos != -1) {
        m_deleteList.move(pos, m_deleteList.size() - 1);
    } else {
        m_deleteList.append(pair);
    }

    //删除
    if (m_deleteList.size() > 50) {
        const auto [fst, snd] = m_deleteList.takeFirst();
        m_deleteListMutex.unlock();

        switch (fst) {
            case ALBUM:
                releaseAlbum(snd);
                break;
            case ARTIST:
                releaseArtist(snd);
                break;
            case MUSIC:
                releaseMusic(snd);
                break;
            case PLAYLIST:
                releasePlayList(snd);
                break;
            default:
                break;
        }
    } else {
        m_deleteListMutex.unlock();
    }
}
