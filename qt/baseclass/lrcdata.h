#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>

using LrcDataPtr = QSharedPointer<class LrcData>;

class LrcData
{

private:
    QList<long long> startList;
    QList<long long> endList;
    QList<QString> textList;


public:
    int id;
    long long startTime;
    long long endTime;
    bool isPlay;
    QList<QString> helpTextList;
    LrcData();

    QJsonObject getJsonObject();

    void append(long long start, long long end, QString text);

    void copy(LrcDataPtr aim);
};

#endif // LRCDATA_H
