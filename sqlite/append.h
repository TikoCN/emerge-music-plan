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

    bool appendDirPlayList(QString url);
    bool appendDirPlayList(QStringList  urlList);

    bool appendUserPlayList(QString name);

    bool appendArtistMusic(QPair<QString, QString> pair);
    bool appendArtistMusic(QList<QPair<QString, QString>> pairList);

<<<<<<< Updated upstream:sqlite/append.h
    bool appendListMusic(QPair<QString, QString> pair);
    bool appendListMusic(QList<QPair<QString, QString>> pairList);
=======
    bool appendPlayListMusic(QPair<QString, QString> pair);
    bool appendPlayListMusic(QList<QPair<QString, QString>> pairList);
    bool appendPlayListMusic(int id, QList<int> musicList);
>>>>>>> Stashed changes:qt/sqlite/append.h

};
#endif // APPNED_H
