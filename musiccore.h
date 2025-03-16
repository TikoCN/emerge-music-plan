#ifndef MUSICCORE_H
#define MUSICCORE_H

#include <QObject>
#include "music.h"
#include "table.h"

class MusicCore : public QObject
{
    Q_OBJECT
public:
    explicit MusicCore(QObject *parent = nullptr);

private:
    QList<Table *> tableList;//播放列表
    QList<Music *> coreList;//音乐核心列表
    QList<LrcData *> lrcList;//歌词
    QList<Music *> musicList;//正在播放列表

signals:
    // 列表增加
    void tableAdd();
};

#endif // MUSICCORE_H
