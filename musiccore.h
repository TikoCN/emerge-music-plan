#ifndef MUSICCORE_H
#define MUSICCORE_H

#include <QObject>
#include "music.h"
#include "table.h"

class MusicCore : public QObject
{
    Q_OBJECT
public:
    static MusicCore* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new MusicCore;
        }
    }


    QList<Table *> tableList;//播放列表
    QList<Music *> coreList;//音乐核心列表

    QList<Music *> getMusicList() const;

    QList<Music *> getCoreList() const;

    // 清楚数据
    void clearDate();

    //获得音乐核心
    void getMusicCore(QList<Music *>musicList);

    //新建播放列表
    Q_INVOKABLE void appendTable(QString tableName, bool isDir = false);

    //将歌曲移动到
    Q_INVOKABLE void tableMoveMusic(int orgTableId, int musicId, int aimTalbeId);


    QList<Table *> getTableList() const;

private:
    static MusicCore* instance;
    explicit MusicCore(QObject *parent = nullptr);

    Q_PROPERTY(QList<Table *> tableList READ getTableList CONSTANT)

    Q_PROPERTY(QList<Music *> coreList READ getCoreList CONSTANT)

signals:
    // 列表增加
    void tableAdd();
};

#endif // MUSICCORE_H
