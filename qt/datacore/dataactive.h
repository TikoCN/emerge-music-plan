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
            delete instance;
    }

    Q_INVOKABLE QList<int> playListShowAllMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowLoveMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowSearchMusic(int playListId, const QString& e);
    Q_INVOKABLE QList<int> musicListSort(const QList<int> &, PlayList::SORT_TYPE sort);

    Q_INVOKABLE void appendPlayList(const QString &name) const;
    Q_INVOKABLE void clearNullItem() const;
    Q_INVOKABLE void playlistAppendMusic(int id, const QList<int>& musicList);

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, bool level);

    Q_INVOKABLE void updateALLNameKey() const;

signals:
    void buildAlbumPlayer();
    void buildArtistPlayer();
    void buildPlayListPlayer();
    void finish();

private:
    explicit DataActive();
    ~DataActive() override;

    static DataActive *instance;
};

#endif // DATAACTIVE_H
