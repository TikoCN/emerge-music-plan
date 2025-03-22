#ifndef ALUMB_H
#define ALUMB_H

#include <QObject>
#include "music.h"

class Alumb : public QObject
{
    Q_OBJECT
public:
    explicit Alumb(QString name);

    QList<Music *> musicList; // 音乐列表
    QString name;
    int id;

    QString getName() const;

    QList<Music *> getMusicList() const;

    int getId() const;

private:
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QList<Music *> musicList READ getMusicList CONSTANT)
    Q_PROPERTY(int id READ getId CONSTANT FINAL)
};

#endif // ALUMB_H
