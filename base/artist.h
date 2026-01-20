#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
<<<<<<< Updated upstream:base/artist.h

class Artist : public QObject
=======
using ArtistPtr = QSharedPointer<class Artist>;

class Artist
>>>>>>> Stashed changes:qt/base/artist.h
{
public:
    explicit Artist(QString name, int id, QString linekey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    long long duration;
    int id;

<<<<<<< Updated upstream:base/artist.h
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
=======
    QJsonObject getJsonObject();
>>>>>>> Stashed changes:qt/base/artist.h
};

#endif // ARTIST_H
