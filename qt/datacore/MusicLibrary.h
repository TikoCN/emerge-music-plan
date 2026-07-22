#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include "baseclass/Music.h"

class MusicLibraryModel;

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    static MusicLibrary &getInstance() {
        static MusicLibrary instance;
        return instance;
    }

    Q_INVOKABLE QList<int> musicListSort(const QList<int> &musicIdList, SORT_TYPE sort);

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, bool level);

    Q_INVOKABLE Music getMusicData(int id);
    Q_INVOKABLE QJsonObject getMusicJson(int id);

    Q_INVOKABLE QStringList getMusicKeys() const;

    Q_INVOKABLE void startClearInvalidData();

    Q_INVOKABLE MusicLibraryModel *model();

private:
    explicit MusicLibrary();
    ~MusicLibrary() override;

    MusicLibraryModel *m_model;
};

#endif // MUSICLIBRARY_H