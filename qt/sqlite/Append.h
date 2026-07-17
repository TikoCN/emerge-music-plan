#ifndef APPNED_H
#define APPNED_H

#include "Core.h"
#include "baseclass/MediaData.h"

class Append {
private:
    Core *core;

public:
    explicit Append(Core *core) : core(core) {
    };

    // 附加条目
    [[nodiscard]] bool appendMusic(const MediaData &data) const;
    [[nodiscard]] bool appendMusic(const QList<MediaData> &data) const;

    [[nodiscard]] bool appendAlbum(const QString &album) const;
    [[nodiscard]] bool appendAlbum(const QStringList &albumList) const;
    [[nodiscard]] bool appendAlbumMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendAlbumMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendAlbumMusic(const QList<QPair<QString, QString> > &pairList) const;

    [[nodiscard]] bool appendArtist(const QString &artist) const;
    [[nodiscard]] bool appendArtist(const QStringList &artistList) const;
    [[nodiscard]] bool appendArtistMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendArtistMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendArtistMusic(const QList<QPair<QString, QString> > &pairList) const;

    [[nodiscard]] bool appendDirPlayList(const QString &url) const;
    [[nodiscard]] bool appendDirPlayList(const QStringList &urlList) const;

    [[nodiscard]] bool appendUserPlayList(const QString &name) const;

    [[nodiscard]] bool appendPlayListMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendPlayListMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendPlayListMusic(const QList<QPair<QString, QString> > &pairList) const;

    [[nodiscard]] bool appendPlayingListMusic(int musicId, int position) const;
    [[nodiscard]] bool appendPlayingListMusic(const QList<int> &musicList, int start) const;
};
#endif // APPNED_H
