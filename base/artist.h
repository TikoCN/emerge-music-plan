#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include "music.h"

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QString name, int id, QString linekey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    int id;

    QString getName() const;

    int getId() const;

    QString getLineKey() const;

    // 得到歌手歌曲总时间
    Q_INVOKABLE QString getStringTime();

    QList<int> getMusicList() const;

private:
    Q_PROPERTY(QString name READ getName CONSTANT FINAL)
    Q_PROPERTY(int id READ getId CONSTANT FINAL)
    Q_PROPERTY(QString lineKey READ getLineKey CONSTANT FINAL)
    Q_PROPERTY(QList<int> musicList READ getMusicList CONSTANT FINAL)
};

#endif // ARTIST_H
