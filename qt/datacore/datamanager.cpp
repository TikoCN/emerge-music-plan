#include "datamanager.h"

#include "sqlite/sqlite.h"
#include "tlog.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "sqlite/sqlite.h"
#include "tlog.h"

DataManager::DataManager()
{
    sql = SQLite::getInstance();
    log = TLog::getInstance();
}

AlbumPtr DataManager::getAlbumCore(int id)
{
    m_albumMutex.lock();

    log->logInfo(QString("获得 Album %1").arg(id));

    AlbumPtr album = nullptr;
    if (m_albumHash.contains(id)) {
        album = m_albumHash.value(id);
    } else {
        album = sql->getAlbum(id);
        if (album != nullptr) {
            m_albumHash.insert(id, album);
        }
    }

    m_albumMutex.unlock();
    deleteOutChace(DataManager::CORE_TYPE::ALBUM, id);
    return album;
}

QJsonObject DataManager::getAlbumJson(int id)
{
    AlbumPtr album = getAlbumCore(id);
    QJsonObject json = album->getJsonObject();

    return json;
}

ArtistPtr DataManager::getArtistCore(int id)
{
    m_artistMutex.lock();
    log->logInfo(QString("获得 Artist %1").arg(id));

    ArtistPtr artist = nullptr;
    if (m_artistHash.contains(id)) {
        artist = m_artistHash.value(id);
    } else {
        artist = sql->getArtist(id);
        if (artist != nullptr) {
            m_artistHash.insert(id, artist);
        }
    }

    m_artistMutex.unlock();
    deleteOutChace(DataManager::CORE_TYPE::ARTIST, id);
    return artist;
}

QJsonObject DataManager::getArtistJson(int id)
{
    ArtistPtr artist = getArtistCore(id);
    QJsonObject json = artist->getJsonObject();

    return json;
}

MusicPtr DataManager::getMusicCore(int id)
{
    m_musicMutex.lock();
    log->logInfo(QString("获得 Music %1").arg(id));

    MusicPtr music = nullptr;
    if (m_musicHash.contains(id)) {
        music = m_musicHash.value(id);
    } else {
        music = sql->getMusic(id);
        if (music != nullptr) {
            m_musicHash.insert(id, music);
        }
    }

    m_musicMutex.unlock();
    deleteOutChace(DataManager::CORE_TYPE::MUSIC, id);
    return music;
}

QList<MusicPtr> DataManager::getMusicCoreList(QList<int> idList)
{
    log->logInfo(QString("获得 Music %1").arg(idList.size()));

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

QJsonObject DataManager::getMusicJson(int id)
{
    MusicPtr music = getMusicCore(id);
    QJsonObject json = music->getJsonObject();
    return json;
}

PlayListPtr DataManager::getPlayListCore(int id)
{
    m_playlistMutex.lock();
    log->logInfo(QString("获得 PlayList %1").arg(id));

    PlayListPtr playlist = nullptr;
    if (m_playlistHash.contains(id)) {
        playlist = m_playlistHash.value(id);
    } else {
        playlist = sql->getList(id);
        if (playlist != nullptr) {
            m_playlistHash.insert(id, playlist);
        }
    }

    m_playlistMutex.unlock();
    deleteOutChace(DataManager::CORE_TYPE::PLAYLIST, id);
    return playlist;
}

QJsonObject DataManager::getPlayListJson(int id)
{
    PlayListPtr playlist = getPlayListCore(id);
    QJsonObject json = playlist->getJsonObject();
    return json;
}

void DataManager::releaseAlbum(int id)
{
    m_albumMutex.lock();
    log->logError(tr("执行释放") +
                                  tr("专辑ID:%1").arg(id));
    m_albumHash.remove(id);
    m_albumMutex.unlock();
}

void DataManager::releaseArtist(int id)
{
    m_artistMutex.lock();
    log->logError(tr("执行释放") +
                                  tr("歌手ID:%1").arg(id));
    m_artistHash.remove(id);
    m_artistMutex.unlock();
}

void DataManager::releaseMusic(int id)
{
    m_musicMutex.lock();
    log->logError(tr("执行释放") +
                                  tr("歌曲ID:%1").arg(id));
    m_musicHash.remove(id);
    m_musicMutex.unlock();
}

void DataManager::releasePlayList(int id)
{
    m_playlistMutex.lock();
    log->logError(tr("执行释放") +
                                  tr("播放列表ID:%1").arg(id));
    m_playlistHash.remove(id);
    m_playlistMutex.unlock();
}

void DataManager::deleteOutChace(DataManager::CORE_TYPE type, int id)
{
    m_deleteListMutex.lock();

    //插入
    QPair<DataManager::CORE_TYPE, int> pair(type, id);
    int pos = m_deleteList.indexOf(pair);
    if (pos != -1) {
        m_deleteList.move(pos, m_deleteList.size() - 1);
    } else {
        m_deleteList.append(pair);
    }

    //删除
    if (m_deleteList.size() > 50) {
        QPair<DataManager::CORE_TYPE, int> deleteDataActive = m_deleteList.takeFirst();
        m_deleteListMutex.unlock();

        switch (deleteDataActive.first) {
        case DataManager::CORE_TYPE::ALBUM:
            releaseAlbum(deleteDataActive.second);
            break;
        case DataManager::CORE_TYPE::ARTIST:
            releaseArtist(deleteDataActive.second);
            break;
        case DataManager::CORE_TYPE::MUSIC:
            releaseMusic(deleteDataActive.second);
            break;
        case DataManager::CORE_TYPE::PLAYLIST:
            releasePlayList(deleteDataActive.second);
            break;
        default:
            break;
        }
    } else {
        m_deleteListMutex.unlock();
    }
}

