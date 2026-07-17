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

    Q_INVOKABLE QList<int> musicListSort(const QList<int> &, SORT_TYPE sort);

    Q_INVOKABLE void clearNullItem();

    Q_INVOKABLE static void appendPlayList(const QString &name);

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, bool level);

    Q_INVOKABLE void updatePlayListName(int playListId, const QString &name);
    Q_INVOKABLE void updatePlayListSort(int playListId, int sort);

    Q_INVOKABLE void updateArtistName(int artistId, const QString &name);
    Q_INVOKABLE void updateArtistSort(int artistId, int sort);

    Q_INVOKABLE void updateAlbumName(int albumId, const QString &name);
    Q_INVOKABLE void updateAlbumSort(int albumId, int sort);

    Q_INVOKABLE void updateALLNameKey() const;

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
