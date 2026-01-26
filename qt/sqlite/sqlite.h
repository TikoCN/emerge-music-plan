#ifndef SQLITE_H
#define SQLITE_H
#include "get.h"

class SQLite : public Get
{
    Q_OBJECT
public:
    static SQLite *instance;
    static SQLite *getInstance();

    static void buildInstance(TLog *tlog){
        if (instance == nullptr) {
            instance = new SQLite(tlog);
        }
    }

    static void freeInstance(){
            delete instance;
    }

    // 获得新的条目
    bool selectNewMusic(const QFileInfoList& infoList, QFileInfoList *newInfoList);

    // 清除不存在的条目
    QList<QString> clearNullMusicItem();
    QList<QString> clearNullPlayListItem();

private:
    explicit SQLite(TLog *log);
    ~SQLite() override;

};

#endif // SQLITE_H
