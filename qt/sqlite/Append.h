#ifndef APPNED_H
#define APPNED_H

#include "Core.h"
#include "baseclass/MediaData.h"

class Append : public Core
{
    Q_OBJECT
public:
    explicit Append(TLog *log, BaseTool *tool)
        :Core(log, tool) {}

    // 附加条目
    bool appendMusic(const MediaData &data);
    bool appendMusic(const QList<MediaData>& data);

    bool appendAlbum(const QString& album);
    bool appendAlbum(const QStringList& albumList);
    bool appendAlbumMusic(int id, const QList<int>& musicList);
    bool appendAlbumMusic(const QPair<QString, QString> &pair);
    bool appendAlbumMusic(const QList<QPair<QString, QString> > &pairList);

    bool appendArtist(const QString& artist);
    bool appendArtist(const QStringList& artistList);
    bool appendArtistMusic(int id, const QList<int>& musicList);
    bool appendArtistMusic(const QPair<QString, QString> &pair);
    bool appendArtistMusic(const QList<QPair<QString, QString> > &pairList);

    bool appendDirPlayList(const QString& url);
    bool appendDirPlayList(const QStringList&  urlList);

    bool appendUserPlayList(const QString &name);

    bool appendPlayListMusic(int id, const QList<int>& musicList);
    bool appendPlayListMusic(const QPair<QString, QString> &pair);
    bool appendPlayListMusic(const QList<QPair<QString, QString> > &pairList);

    bool appendPlayingListMusic(int musicId, int position);
    bool appendPlayingListMusic(const QList<int> &musicList, int start);
};
#endif // APPNED_H
