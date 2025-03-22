#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include "music.h"

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QString name);

    QList<Music *> musicList; // 音乐列表
    QString name;
    int id;

    QList<Music *> getMusicList() const;
    QString getName() const;

    int getId() const;

private:
    Q_PROPERTY(QList<Music *> musicList READ getMusicList CONSTANT FINAL)
    Q_PROPERTY(QString name READ getName CONSTANT FINAL)
    Q_PROPERTY(int id READ getId CONSTANT FINAL)
};

#endif // ARTIST_H
