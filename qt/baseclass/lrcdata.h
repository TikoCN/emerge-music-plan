#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>

using LrcDataPtr = QSharedPointer<class LrcData>;

class LrcData {
private:
    QList<long long> startList;
    QList<long long> endList;
    QList<QString> textList;

public:
    int id;
    long long startTime;
    long long endTime;
    QList<QString> helpTextList;

    LrcData();

    [[nodiscard]] QJsonObject getJsonObject() const;

    void append(long long start, long long end, const QString &text);

    void copy(const LrcDataPtr &aim);
};

#endif // LRCDATA_H
