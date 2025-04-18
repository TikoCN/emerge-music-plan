#include "get.h"

QStringList Get::getArtistKeyList()
{
    QStringList keyList;
    try {
        const char *sql = "SELECT name FROM artist";

        r = sqlite3_exec(db, sql, [](void *data, int argc, char **argv, char **azColName)->int{
            QStringList *keyList = static_cast<QStringList *>(data);
            keyList->append(QString(*argv));
            return SQLITE_OK;
        }, &keyList, &error);

        if (r != SQLITE_OK) logError("得到 artist 所有 name 失败");
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

// QList<Artist *> Get::getArtist(QString key)
// {

// }

// Artist *Get::getArtist(int id)
// {

// }

// QList<Artist *> Get::getArtist(QList<int> idList)
// {

// }

// QStringList Get::getAlbumKeyList()
// {

// }

// QList<Album *> Get::getAlbum(QString key)
// {

// }

// Album *Get::getAlbum(int id)
// {

// }

// QList<Album *> Get::getAlbum(QList<int> idList)
// {

// }

// Music *Get::getMusic(int id)
// {

// }

// QList<Music *> Get::getMusic(QList<int> idList)
// {

// }

// QStringList Get::getAllList()
// {

// }

// QList<Table *> Get::getList(int id)
// {

// }
