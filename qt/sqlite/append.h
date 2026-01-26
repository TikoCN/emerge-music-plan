#ifndef APPNED_H
#define APPNED_H

#include "core.h"
#include "baseclass/mediadata.h"

class Append : public Core
{
    Q_OBJECT
public:
    explicit Append(TLog *log)
        :Core(log) {}

    // 附加条目
    bool appendMusic(const MediaData &data);
    bool appendMusic(const QList<MediaData>& data);

    bool appendAlbum(const QString& album);
    bool appendAlbum(const QStringList& albumList);

    bool appendArtist(const QString& artist);
    bool appendArtist(const QStringList& artistList);

    bool appendDirPlayList(const QString& url);
    bool appendDirPlayList(const QStringList&  urlList);

    bool appendUserPlayList(const QString &name);

    bool appendArtistMusic(const QPair<QString, QString>& pair);
    bool appendArtistMusic(const QList<QPair<QString, QString>>& pairList);
    bool appendArtistMusic(int id, const QList<int>& musicList);

    bool appendPlayListMusic(const QPair<QString, QString>& pair);
    bool appendPlayListMusic(const QList<QPair<QString, QString>>& pairList);
    bool appendPlayListMusic(int id, const QList<int>& musicList);

};
#endif // APPNED_H
