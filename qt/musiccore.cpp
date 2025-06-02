#include "musiccore.h"
#include "sqlite/sqlite.h"
#include "tlog.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "setting.h"

void MusicCore::appendTable(QString name)
{
    SQLite *sql = SQLite::getInstance();
    sql->appendUserTable(name);
}

void MusicCore::clearNullItem()
{
    SQLite::getInstance()->clearNullMusicItem();
    SQLite::getInstance()->clearNullPlayListItem();
}

Table *MusicCore::getTable(int id)
{
    TLog::getInstance()->logInfo(QString("获得 Table %1").arg(id));

    Table *table = nullptr;
    if (m_tableHash.contains(id)) {
        table = m_tableHash.value(id);
        table->increment();
        return table;
    }

    SQLite *sql = SQLite::getInstance();
    table = sql->getList(id);
    if (table != nullptr) {
        table->moveToThread(this->thread());
        m_tableHash.insert(id, table);
        table->increment();
        return table;
    }
    return nullptr;
}

void MusicCore::releaseTable(int id)
{
    TLog::getInstance()->logRelease(QString("释放 Table %1").arg(id));

    Table *table = nullptr;
    if (m_tableHash.contains(id))
        table = m_tableHash.value(id);
    else
        return;

    if (table->decrement()) {
        m_tableHash.remove(id);
        delete table;
    }
}

Album *MusicCore::getAlbum(int id)
{
    TLog::getInstance()->logInfo(QString("获得 Album %1").arg(id));

    Album *album = nullptr;
    if (m_albumHash.contains(id)) {
        album = m_albumHash.value(id);
        album->increment();
        return album;
    }

    SQLite *sql = SQLite::getInstance();
    album = sql->getAlbum(id);
    if (album != nullptr) {
        album->moveToThread(this->thread());
        m_albumHash.insert(id, album);
        album->increment();
        return album;
    }
    return nullptr;
}

void MusicCore::releaseAlbum(int id)
{
    TLog::getInstance()->logError(QString("释放 Album %1").arg(id));

    Album *album = nullptr;
    if (m_albumHash.contains(id))
        album = m_albumHash.value(id);
    else
        return;

    if (album->decrement()) {
        m_albumHash.remove(id);
        delete album;
    }
}

Artist *MusicCore::getArtist(int id)
{
    TLog::getInstance()->logInfo(QString("获得 Artist %1").arg(id));

    Artist *artist = nullptr;
    if (m_artistHash.contains(id)) {
        artist = m_artistHash.value(id);
        artist->increment();
        return artist;
    }

    SQLite *sql = SQLite::getInstance();
    artist = sql->getArtist(id);
    if (artist != nullptr) {
        artist->moveToThread(this->thread());
        m_artistHash.insert(id, artist);
        artist->increment();
        return artist;
    }
    return nullptr;
}

void MusicCore::releaseArtist(int id)
{
    TLog::getInstance()->logRelease(QString("获得 Artist %1").arg(id));

    Artist *artist = nullptr;
    if (m_artistHash.contains(id))
        artist = m_artistHash.value(id);
    else
        return;

    if (artist->decrement()) {
        m_artistHash.remove(id);
        delete artist;
    }
}

Music *MusicCore::getMusic(int id)
{
    TLog::getInstance()->logInfo(QString("获得 Music %1").arg(id));

    Music *music = nullptr;
    if (m_musicHash.contains(id)) {
        music = m_musicHash.value(id);
        music->increment();
        return music;
    }

    SQLite *sql = SQLite::getInstance();
    music = sql->getMusic(id);
    if (music != nullptr) {
        music->moveToThread(this->thread());
        m_musicHash.insert(id, music);
        music->increment();
        return music;
    }
    return nullptr;
}

QList<Music *> MusicCore::getMusic(QList<int> idList)
{
    TLog::getInstance()->logInfo(QString("获得 Music %1").arg(idList.size()));

    QList<Music *> musicList;
    QList<int> newIdList;
    for (int i = 0; i < idList.size(); ++i) {
        if (!m_musicHash.contains(idList[i]))
            newIdList.append(idList[i]);
        else
            musicList.append(m_musicHash.value(idList[i]));
    }
    QHash<int, Music *> hash = SQLite::getInstance()->getMusic(newIdList);
    m_musicHash.insert(hash);

    musicList.append(hash.values());
    return musicList;
}

void MusicCore::releaseMusic(int id)
{
    TLog::getInstance()->logRelease(QString("获得 Music %1").arg(id));

    Music *music = nullptr;
    if (m_musicHash.contains(id))
        music = m_musicHash.value(id);
    else
        return;

    if (music->decrement()) {
        m_musicHash.remove(id);
        delete music;
    }
}

void MusicCore::releaseMusic(QList<int> idList)
{
    for (int i = 0; i < idList.size(); ++i)
        releaseMusic(idList[i]);
}

void MusicCore::loadMusic(QList<int> idList)
{
    QList<int> newIdList;
    for (int i = 0; i < idList.size(); ++i) {
        if (!m_musicHash.contains(idList[i]))
            newIdList.append(idList[i]);
    }
    QHash<int, Music *> hash = SQLite::getInstance()->getMusic(newIdList);
    m_musicHash.insert(hash);
}

void MusicCore::tableAppendMusic(int id, QList<int> musicList)
{
    Table *table = getTable(id);
    if (table != nullptr) {
        table->musicList.append(musicList);
        releaseTable(id);
    }

    SQLite::getInstance()->appendTableMusic(id, musicList);
}

QList<int> MusicCore::selectSearchMusic(QList<int> idList, QString e)
{
    QList<Music *> musicList = getMusic(idList);
    QList<int> newIdList;

    for (int i = 0; i < musicList.size(); ++i) {
        if (musicList[i]->isSearch(e))
            newIdList.append(musicList[i]->id);
    }

    releaseMusic(idList);
    return newIdList;
}

QList<int> MusicCore::sortMusic(QList<int> idList, Table::SORT_TYPE sort)
{
    QList<Music *> musicList = getMusic(idList);
    QList<int> newIdList;

    switch (sort) {
    case Table::SORT_TITTLE_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->title > b->title;
        });
        break;
    case Table::SORT_ALUMB_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->album > b->album;
        });
        break;
    case Table::SORT_ATRIST_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->getArtist() > b->getArtist();
        });
        break;
    case Table::SORT_DURATION_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->duration > b->duration;
        });
        break;
    case Table::SORT_LEVEL_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->level > b->level;
        });
        break;
    case Table::SORT_LAST_EDIT_TIME_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->lastEditTime > b->lastEditTime;
        });
        break;
    case Table::SORT_PLAY_NUMBER_ASC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->playNumber > b->playNumber;
        });
        break;
    case Table::SORT_TITTLE_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->title < b->title;
        });
        break;
    case Table::SORT_ALUMB_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->album < b->album;
        });
        break;
    case Table::SORT_ATRIST_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->getArtist() < b->getArtist();
        });
        break;
    case Table::SORT_DURATION_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->duration < b->duration;
        });
        break;
    case Table::SORT_LEVEL_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->level < b->level;
        });
        break;
    case Table::SORT_LAST_EDIT_TIME_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->lastEditTime < b->lastEditTime;
        });
        break;
    case Table::SORT_PLAY_NUMBER_DESC:
        std::sort(musicList.begin(), musicList.end(), [](Music *a, Music *b)->bool{
            return a->playNumber < b->playNumber;
        });
        break;
    default:
        break;
    }

    for (int i = 0; i < musicList.size(); ++i) {
        newIdList.append(musicList[i]->id);
    }

    releaseMusic(idList);
    return newIdList;
}

QList<int> MusicCore::selectLoveMusic(QList<int> idList)
{
    QList<Music *> musicList = getMusic(idList);
    QList<int> newIdList;

    for (int i = 0; i < musicList.size(); ++i) {
        if (musicList[i]->isLove)
            newIdList.append(musicList[i]->id);
    }

    releaseMusic(idList);
    return newIdList;
}

QString MusicCore::getArtistName(int id)
{
    if (m_artistHash.contains(id))
        return m_artistHash.value(id)->name;
    return "";
}

QString MusicCore::getAlbumName(int id)
{
    if (m_albumHash.contains(id))
        return m_albumHash.value(id)->name;
    return "";
}

bool MusicCore::setArtistIsNoCover(int id, bool b)
{
    if (m_artistHash.contains(id)){
        m_artistHash.value(id)->isNoCover = b;
        return true;
    }
    return false;
}

bool MusicCore::setAlbumIsNoCover(int id, bool b)
{
    if (m_albumHash.contains(id)){
        m_albumHash.value(id)->isNoCover = b;
        return true;
    }
        return false;
}

bool MusicCore::setMusicIsNoCover(int id, bool b)
{
    if (m_musicHash.contains(id)){
        m_musicHash.value(id)->isNoCover = b;
        return true;
    }
    return false;
}

MusicCore::MusicCore(QObject *parent)
    : QObject{parent}
{

}

