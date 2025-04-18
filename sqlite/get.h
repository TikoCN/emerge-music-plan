#ifndef GET_H
#define GET_H

#include "core.h"
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
#include "base/table.h"

class Get : virtual public Core
{
public:
    QStringList getArtistKeyList();
    QList<Artist *> getArtist(QString key);
    Artist * getArtist(int id);
    QList<Artist *> getArtist(QList<int> idList);

    QStringList getAlbumKeyList();
    QList<Album *> getAlbum(QString key);
    Album * getAlbum(int id);
    QList<Album *> getAlbum(QList<int> idList);

    Music * getMusic(int id);
    QList<Music *> getMusic(QList<int> idList);

    QStringList getAllList();
    QList<Table *> getList(int id);
};

#endif // GET_H
