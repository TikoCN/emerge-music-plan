#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QString name, int id, QString linekey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    long long duration;
    int id;

    QString getName() const;

    int getId() const;

    QString getLineKey() const;

    QList<int> getMusicList() const;

    long long getDuration() const;

    void setName(const QString &newName);

signals:
    void nameChanged();

private:

    Q_PROPERTY(int id READ getId CONSTANT FINAL)
    Q_PROPERTY(QString lineKey READ getLineKey CONSTANT FINAL)
    Q_PROPERTY(QList<int> musicList READ getMusicList CONSTANT FINAL)
    Q_PROPERTY(long long duration READ getDuration CONSTANT FINAL)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
};

#endif // ARTIST_H
