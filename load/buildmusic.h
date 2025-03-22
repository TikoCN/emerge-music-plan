#ifndef BUILDMUSIC_H
#define BUILDMUSIC_H

#include <QObject>
#include "base/music.h"

class BuildMusic : public QObject
{
    Q_OBJECT
private:
    QThread* thread;
public:
    BuildMusic();
    ~BuildMusic();

    // 加载音乐核心数据
    void loadMusicCore();

signals:
    //音乐核心完成
    void loadedMusicCore(QList<Music *>, QList<QList<Music *>>);

    //完成任务
    void finishMusicCore();
};

#endif // BUILDMUSIC_H
