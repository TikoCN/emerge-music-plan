#ifndef APPNED_H
#define APPNED_H

#include "core.h"
#include "base/mediadata.h"

class Append : public Core
{
    Q_OBJECT
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
    bool appendArtistMusic(int id, QList<int> musicList);

    bool appendTableMusic(QPair<QString, QString> pair);
    bool appendTableMusic(QList<QPair<QString, QString>> pairList);
    bool appendTableMusic(int id, QList<int> musicList);

};
#endif // APPNED_H
