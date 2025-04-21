#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QSet>

class Album : public QObject
{
    Q_OBJECT
public:
    explicit Album(QString name, int id, QString lineKey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    QSet<QString> artistSet;
    long long duration;
    int id;

    QString getName() const;

    int getId() const;

    QString getLineKey() const;

    // 得到专辑作者列表
    Q_INVOKABLE QString getArtist();

    QList<int> getMusicList() const;

    long long getDuration() const;

    void setName(const QString &newName);

signals:
    void durationChanged();

    void nameChanged();

private:
    Q_PROPERTY(int id READ getId CONSTANT FINAL)
    Q_PROPERTY(QString lineKey READ getLineKey CONSTANT FINAL)
    Q_PROPERTY(QList<int> musicList READ getMusicList CONSTANT FINAL)
    Q_PROPERTY(long long duration READ getDuration CONSTANT FINAL)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
};
#endif // ALBUM_H
