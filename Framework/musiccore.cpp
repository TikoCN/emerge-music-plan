#include "musiccore.h"
#include "sqlite/sqlite.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

void MusicCore::appendTable(QString name)
{
    SQLite *sql = SQLite::getInstance();
    sql->appendUserTable(name);
}

Table *MusicCore::getTable(int id)
{
    if (m_tableHash.contains(id)) {
        return m_tableHash.value(id);
    }

    SQLite *sql = SQLite::getInstance();
    Table *table = sql->getList(id);
    if (table != nullptr) {
        table->moveToThread(this->thread());
        m_tableHash.insert(id, table);
    }
    return table;
}

Album *MusicCore::getAlbum(int id)
{
    if (m_albumHash.contains(id)) {
        return m_albumHash.value(id);
    }

    SQLite *sql = SQLite::getInstance();
    Album *album = sql->getAlbum(id);
    if (album != nullptr) {
        album->moveToThread(this->thread());
        m_albumHash.insert(id, album);
    }
    return album;
}

Artist *MusicCore::getArtist(int id)
{
    if (m_artistHash.contains(id)) {
        return m_artistHash.value(id);
    }

    SQLite *sql = SQLite::getInstance();
    Artist *artist = sql->getArtist(id);
    if (artist != nullptr) {
        artist->moveToThread(this->thread());
        m_artistHash.insert(id, artist);
    }
    return artist;
}

Music *MusicCore::getMusic(int id)
{
    if (m_musicHash.contains(id)) {
        return m_musicHash.value(id);
    }

    SQLite *sql = SQLite::getInstance();
    Music *music = sql->getMusic(id);
    if (music != nullptr) {
        music->moveToThread(this->thread());
        m_musicHash.insert(id, music);
    }
    return music;
}

QList<Music *> MusicCore::getMusic(QList<int> idList)
{
    QList<int> newIdList;
    for (int i = 0; i < idList.size(); ++i) {
        if (!m_musicHash.contains(idList[i]))
            newIdList.append(idList[i]);
    }
    QHash<int, Music *> hash = SQLite::getInstance()->getMusic(newIdList);
    m_musicHash.insert(hash);

    return hash.values();
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

QList<int> MusicCore::selectSearchMusic(QList<int> idList, QString e)
{
    QList<Music *> musicList = getMusic(idList);
    QList<int> newIdList;

    for (int i = 0; i < musicList.size(); ++i) {
        if (musicList[i]->isSearch(e))
            newIdList.append(musicList[i]->id);
    }

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

    return newIdList;
}

MusicCore::MusicCore(QObject *parent)
    : QObject{parent}
{

}

