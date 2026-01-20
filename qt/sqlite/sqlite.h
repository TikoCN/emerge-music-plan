#ifndef SQLITE_H
#define SQLITE_H
#include "get.h"
#include "append.h"
#include "update.h"
#include <sqlite3.h>
#include <QObject>

class SQLite : public Get
{
    Q_OBJECT
public:
    static SQLite *instance;
    static SQLite *getInstance();

    static void buildInstance(){
        if (instance == nullptr) {
            instance = new SQLite;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    // 获得新的条目
    bool selectNewMusic(QFileInfoList infoList, QFileInfoList *newInfoList);

    // 清除不存在的条目
    QList<QString> clearNullMusicItem();
    QList<QString> clearNullPlayListItem();

private:
    SQLite();
    ~SQLite();

};

#endif // SQLITE_H
