#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include "baseclass/Music.h"
#include "datacore/DataLoader.h"

class MusicLibraryModel;

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    static MusicLibrary &getInstance() {
        static MusicLibrary instance;
        return instance;
    }

    Q_INVOKABLE QList<int> listSort(const QList<int> &musicIdList, SORT_TYPE sort);

    Q_INVOKABLE void updateLove(int musicId, bool isLove);
    Q_INVOKABLE void updateLevel(int musicId, bool level);

    Q_INVOKABLE Music getMusicData(int id);
    Q_INVOKABLE QJsonObject getJson(int id);

    Q_INVOKABLE QStringList getKeys() const;

    Q_INVOKABLE void startClearInvalidData();

    Q_INVOKABLE MusicLibraryModel *model();

    Q_INVOKABLE DataLoader *loader();

    Q_INVOKABLE void loadByKey(const QString &key);
    Q_INVOKABLE void loadMoreByKey(int index, const QString &key);

private:
    explicit MusicLibrary();
    ~MusicLibrary() override;

    MusicLibraryModel *m_model;
    DataLoader *m_loader;
};

#endif // MUSICLIBRARY_H