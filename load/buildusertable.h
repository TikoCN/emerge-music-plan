#ifndef BUILDUSERTABLE_H
#define BUILDUSERTABLE_H

#include <QObject>
#include "base/music.h"

class BuildUserTable : public QObject
{
    Q_OBJECT
private:
    QThread* thread;
public:
    BuildUserTable();
    ~BuildUserTable();

    // 加载用户列表
    void loadUserTable();

signals:
    //音乐核心完成
    void loadedUserTable(QList<Music *>, int);

    void finishUserTable();
};

#endif // BUILDUSERTABLE_H
