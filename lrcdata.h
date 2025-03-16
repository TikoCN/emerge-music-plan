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
    bool isPlay;

private:
    QList<long long> startList;
    QList<long long> endList;
    QList<QString> textList;

public:
    LrcData();

    void append(long long start, long long end, QString text);

    int getId() const;

    QList<long long> getStartList() const;

    QList<long long> getEndList() const;

    qint64 getStartTime() const;

    QList<QString> getTextList() const;

private:
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(qint64 startTime READ getStartTime CONSTANT)
    Q_PROPERTY(QList<long long> startList READ getStartList CONSTANT)
    Q_PROPERTY(QList<long long> endList READ getEndList CONSTANT)
    Q_PROPERTY(QList<QString> textList READ getTextList CONSTANT)

signals:
    void update();
};

#endif // LRCDATA_H
