#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include "referencecount.h"

class Artist : public QObject, public ReferenceCount
{
    Q_OBJECT
public:
    explicit Artist(QString name, int id, QString linekey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    long long duration;
    int id;
    bool isNoCover;

    QString getName() const;

    int getId() const;

    QString getLineKey() const;

    QList<int> getMusicList() const;

    long long getDuration() const;

    void setName(const QString &newName);

    bool getIsNoCover() const;

signals:
    void nameChanged();

private:

    Q_PROPERTY(int id READ getId CONSTANT FINAL)
    Q_PROPERTY(QString lineKey READ getLineKey CONSTANT FINAL)
    Q_PROPERTY(QList<int> musicList READ getMusicList CONSTANT FINAL)
    Q_PROPERTY(long long duration READ getDuration CONSTANT FINAL)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool isNoCover READ getIsNoCover CONSTANT FINAL)
};

#endif // ARTIST_H
