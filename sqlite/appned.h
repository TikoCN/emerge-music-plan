#ifndef APPNED_H
#define APPNED_H

#include "core.h"
#include "base/mediadata.h"

class Append : virtual public Core
{
public:
    // 附加条目
    bool appendMusic(MediaData data);
    bool appendMusic(QList<MediaData> data);

    bool appendAlbum(QString album);
    bool appendAlbum(QStringList albumList);

    bool appendArtist(QString artist);
    bool appendArtist(QStringList artistList);

    bool appendDirTable(QString url);
    bool appendDirTable(QStringList  urlList);

    bool appendUserTable(QString name);

    bool appendArtistMusic(QPair<QString, QString> pair);
    bool appendArtistMusic(QList<QPair<QString, QString>> pairList);

    bool appendListMusic(QPair<QString, QString> pair);
    bool appendListMusic(QList<QPair<QString, QString>> pairList);

};
#endif // APPNED_H
