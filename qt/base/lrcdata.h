#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>
class LrcData : public QObject
{
    Q_OBJECT
public:
    int id;
    long long startTime;
    long long endTime;
    bool isPlay;
    QList<QString> helpTextList;

private:
    QList<long long> startList;
    QList<long long> endList;
    QList<QString> textList;

public:
    LrcData();

    void append(long long start, long long end, QString text);

    void copy(LrcData *aim);

    int getId() const;

    QList<long long> getStartList() const;

    QList<long long> getEndList() const;

    QList<QString> getTextList() const;

    QList<QString> getHelpTextList() const;

    long long getStartTime() const;

    long long getEndTime() const;

private:
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QList<long long> startList READ getStartList CONSTANT)
    Q_PROPERTY(QList<long long> endList READ getEndList CONSTANT)
    Q_PROPERTY(QList<QString> textList READ getTextList CONSTANT)
    Q_PROPERTY(QList<QString> helpTextList READ getHelpTextList CONSTANT)

    Q_PROPERTY(long long startTime READ getStartTime CONSTANT FINAL)

    Q_PROPERTY(long long endTime READ getEndTime CONSTANT FINAL)

signals:
    void update();
    void helpTextListChanged();
};

#endif // LRCDATA_H
