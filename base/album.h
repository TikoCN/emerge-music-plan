#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include "music.h"

class Album : public QObject
{
    Q_OBJECT
public:
    explicit Album(QString name);

    QList<Music *> musicList; // 音乐列表
    QString name;
    QString lineKey;
    QSet<QString> artistSet;
    int id;

    QString getName() const;

    QList<Music *> getMusicList() const;

    int getId() const;

    QString getLineKey() const;

    // 得到专辑总时间
    Q_INVOKABLE QString getStringTime();

    // 得到专辑作者列表
    Q_INVOKABLE QString getArtist();

private:
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QList<Music *> musicList READ getMusicList CONSTANT)
    Q_PROPERTY(int id READ getId CONSTANT FINAL)
    Q_PROPERTY(QString lineKey READ getLineKey CONSTANT FINAL)
};
#endif // ALBUM_H
