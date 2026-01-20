#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <QObject>
#include "baseclass/lrcdata.h"
#include <QJsonArray>

class FileManagement : public QObject
{
    Q_OBJECT
private:
    static FileManagement* instance;
    explicit FileManagement();
public:

    static FileManagement* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new FileManagement;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    // MUSIC FILE
    Q_INVOKABLE void writeMusicToFile(QStringList key, QStringList value, int musicId);
    Q_INVOKABLE QJsonArray getMusicAllTaglib(int musicId);

    // LRC FILE
    Q_INVOKABLE QString getMusicLrcUrl(int musicId);
    Q_INVOKABLE QString getMusicLrcData(int musicId);
    Q_INVOKABLE void wrtiLrcData(int musicId, QString lrcData);
    QList<LrcDataPtr> getMusicLyricsData(int musicId);

    // DIR
    Q_INVOKABLE void openPlayListDir(int playListId);
};

#endif // FILEMANAGEMENT_H
