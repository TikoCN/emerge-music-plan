#ifndef DATAACTIVE_H
#define DATAACTIVE_H
#include "DataManager.h"

class DataActive : public DataManager {
    Q_OBJECT

public:
    static DataActive &getInstance() {
        static DataActive instance;
        return instance;
    }

    QList<int> musicListSort(const QList<int> &, SORT_TYPE sort);

    void clearNullItem();

    static void appendPlayList(const QString &name);

    void updateMusicLove(int musicId, bool isLove);
    void updateMusicLevel(int musicId, bool level);

    void updatePlayListName(int playListId, const QString &name);
    void updatePlayListSort(int playListId, int sort);

    void updateArtistName(int artistId, const QString &name);
    void updateArtistSort(int artistId, int sort);

    void updateAlbumName(int albumId, const QString &name);
    void updateAlbumSort(int albumId, int sort);

    void updateALLNameKey() const;

signals:
    void buildAlbumPlayer();
    void buildArtistPlayer();
    void buildPlayListPlayer();
    void finish();

private:
    explicit DataActive();
    ~DataActive() override;
};

#endif // DATAACTIVE_H
