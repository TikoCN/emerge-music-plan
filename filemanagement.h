#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <QObject>
#include "base/lrcdata.h"
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

    //将数据写入文件
    Q_INVOKABLE void writeMusicToFile(QStringList key, QStringList value, int musicId);

    Q_INVOKABLE QString getMusicLrcUrl(int musicId);

    //获得歌词文件数据
    Q_INVOKABLE QList<LrcData *> getMusicLyricsData(int musicId);

    Q_INVOKABLE QJsonArray getMusicAllTaglib(int musicId);

    Q_INVOKABLE void openPlayListDir(int playListId);
};

#endif // FILEMANAGEMENT_H
