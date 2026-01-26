#include "typeconversion.h"
#include <QDateTime>

TypeConversion::TypeConversion(QObject *parent)
    : QObject{parent}
{

}

QString TypeConversion::durationToTimeStringNoMax(const long long duration)
{
    const qint64 totalSeconds = duration / 1000;

    const qint64 totalHours = totalSeconds / 3600;
    const qint64 minutes = (totalSeconds % 3600) / 60;
    const qint64 seconds = totalSeconds % 60;

    return QString("%1:%2:%3")
        .arg(totalHours, 2, 10, QChar(' '))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

QString TypeConversion::timeToString(const long long time)
{
    QDateTime dataTime;
    dataTime.setMSecsSinceEpoch(time);
    dataTime = dataTime.toUTC();
    return dataTime.toString("hh:mm:ss");
}

QString TypeConversion::intListToString(const QList<int>& list) {
    QStringList strList;
    for (const int num : list) {
        strList.append(QString::number(num));
    }
    return "Int:" + strList.join(";");
}

QString TypeConversion::intListToString(const QList<long long>& list) {
    QStringList strList;
    for (const long long num : list) {
        strList.append(QString::number(num));
    }
    return "Int:" + strList.join(";");
}

QString TypeConversion::stringListToString(const QStringList& list)
{
    return "String:" + list.join(";");
}

QList<int> TypeConversion::stringToIntList(const QString& str) {
    if(!str.contains("Int:"))
        return {};

    QStringList strList = str.split("Int:")[1].split(";");

    QList<int> list;
    for (int i = 0; i < strList.length(); i++) {
        list.append(strList[i].toInt());
    }
    return list;
}

QList<long long> TypeConversion::stringToLongList(const QString& str) {
    if(!str.contains("Int:"))
        return {};

    QStringList strList = str.split("Int:")[1].split(";");

    QList<long long> list;
    for (int i = 0; i < strList.length(); i++) {
        list.append(strList[i].toInt());
    }
    return list;
}

QList<QString> TypeConversion::stringToStringList(const QString &str)
{
    if(!str.contains("String:"))
        return {};
    return str.split("String:")[1].split(";").toList();
}
