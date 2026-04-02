#ifndef LRCDATA_H
#define LRCDATA_H

#include <QObject>
#include "macro/ConstantProperty.h"

using LrcDataPtr = QSharedPointer<class LrcData>;

class LrcData {
    Q_GADGET
public:
    QT_CONSTANT_PROPERTY(QVector<QString>, textList, TextList)
    QT_CONSTANT_PROPERTY(QVector<QString>, helpTextList, HelpTextList)
    QT_CONSTANT_PROPERTY(QVector<long long>, startList, StartList)
    QT_CONSTANT_PROPERTY(QVector<long long>, endList, EndList)
    QT_CONSTANT_PROPERTY(long long, startTime, StartTime)
    QT_CONSTANT_PROPERTY(long long, endTime, EndTime)
    QT_CONSTANT_PROPERTY(int, id, Id)

    LrcData();

    [[nodiscard]] QJsonObject getJsonObject() const;

    void append(long long start, long long end, const QString &text);

    void copy(const LrcDataPtr &aim);
};

Q_DECLARE_METATYPE(LrcData)

#endif // LRCDATA_H
