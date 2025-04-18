#include "musiccore.h"
#include "base.h"
#include "sqlite/sqlite.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

QStringList MusicCore::getArtistKeyList()
{
    return SQLite::getInstance()->getArtistKeyList();
}

QStringList MusicCore::getAlbumKeyList()
{
    return SQLite::getInstance()->getAlbumKeyList();
}

QHash<int, QString> MusicCore::getAllList()
{
    return SQLite::getInstance()->getAllList();
}

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
        m_musicHash.insert(id, music);
    }
    return music;
}

MusicCore::MusicCore(QObject *parent)
    : QObject{parent}
{

}

