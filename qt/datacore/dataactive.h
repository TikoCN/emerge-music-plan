#ifndef DATAACTIVE_H
#define DATAACTIVE_H
#include "datamanager.h"

class DataActive : public DataManager
{
    Q_OBJECT

public:
    static DataActive* getInstance() {
        return instance;
    }

    static void buildInstance() {
        if (instance == nullptr) {
            instance = new DataActive;
        }
    }

    static void freeInstance() {
        if (instance != nullptr) {
            delete instance;
        }
    }

    Q_INVOKABLE QList<int> playListShowAllMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowLoveMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowSearchMusic(int playListId, QString e);
    Q_INVOKABLE QList<int> musicListSort(QList<int>, PlayList::SORT_TYPE sort);

    Q_INVOKABLE void appendPlayList(QString name);
    Q_INVOKABLE void clearNullItem();
    Q_INVOKABLE void playlistAppendMusic(int id, QList<int> musicList);

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, bool level);
    Q_INVOKABLE void updateAllNameKey();

signals:
    void buildAlbumPlayer();
    void buildArtistPlayer();
    void buildPlayListPlayer();
    void finish();

private:
    explicit DataActive();
    ~DataActive();

    static DataActive *instance;
};

#endif // DATAACTIVE_H
