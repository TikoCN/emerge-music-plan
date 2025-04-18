#ifndef SQLITE_H
#define SQLITE_H
#include "get.h"
#include "appned.h"
#include "update.h"
#include <sqlite3.h>
#include <QObject>

class SQLite : public QObject, virtual public Update, virtual public Get, virtual public Append
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

private:
    SQLite();
    ~SQLite();

};

#endif // SQLITE_H
