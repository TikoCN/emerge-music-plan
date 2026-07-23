#ifndef PLAYLISTLIBRARY_H
#define PLAYLISTLIBRARY_H

#include <QObject>
#include "baseclass/Playlist.h"
#include "DataLoader.h"

class PlayListLibraryModel;

class PlayListLibrary : public QObject {
    Q_OBJECT

public:
    static PlayListLibrary &getInstance() {
        static PlayListLibrary instance;
        return instance;
    }

    Q_INVOKABLE void appendPlayList(const QString &name);

    Q_INVOKABLE void updatePlayListName(int playListId, const QString &name);
    Q_INVOKABLE void updatePlayListSort(int playListId, int sort);

    Q_INVOKABLE PlayList getPlayListData(int id);
    Q_INVOKABLE QJsonObject getPlayListJson(int id);

    Q_INVOKABLE int checkPlayListName(const QString &name) const;

    Q_INVOKABLE bool movePlayListMusic(const QString &playListName, const QString &playListNameNew) const;
    Q_INVOKABLE bool addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const;

    Q_INVOKABLE QString getAllList() const;
    Q_INVOKABLE void clearNullItem();
    Q_INVOKABLE void updateALLNameKey() const;

    Q_INVOKABLE PlayListLibraryModel *model();

    Q_INVOKABLE DataLoader *loader();

    Q_INVOKABLE void loadByKey(const QString &key);
    Q_INVOKABLE void loadMoreByKey(int index, const QString &key);

signals:
    void buildPlayListPlayer();
    void finish();

private:
    explicit PlayListLibrary();
    ~PlayListLibrary() override;

    PlayListLibraryModel *m_model;
    DataLoader *m_loader;
};

#endif // PLAYLISTLIBRARY_H