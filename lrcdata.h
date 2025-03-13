#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>
class LrcData : public QObject
{
    Q_OBJECT
public:
    int id;
    qint64 startTime;
    qint64 endTime;
    QList<long long> startList;
    QList<long long> endList;
    QList<QString> textList;
    bool isPlay;

public:
    LrcData();

    void append(long long start, long long end, QString text);

    int getId() const;
    void setId(int newId);

    QList<long long> getStartList() const;

    QList<long long> getEndList() const;

    qint64 getStartTime() const;

    QList<QString> getTextList() const;
    void setTextList(const QList<QString> &newTextList);

    bool getIsPlay() const;
    void setIsPlay(bool newIsPlay);

signals:
    void isPlayChanged();

    void idChanged();

    void update();

    void lineChanged();

    void textListChanged();

private:
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QList<long long> startList READ getStartList CONSTANT FINAL)
    Q_PROPERTY(QList<long long> endList READ getEndList CONSTANT FINAL)
    Q_PROPERTY(qint64 startTime READ getStartTime CONSTANT FINAL)
    Q_PROPERTY(QList<QString> textList READ getTextList WRITE setTextList NOTIFY textListChanged FINAL)
    Q_PROPERTY(bool isPlay READ getIsPlay WRITE setIsPlay NOTIFY isPlayChanged FINAL)
};

#endif // LRCDATA_H
