#include "typeconversion.h"
#include <QDateTime>

TypeConversion::TypeConversion(QObject *parent)
    : QObject{parent}
{

}

QString TypeConversion::durationToTimeStringNoMax(long long duration) const
{
    qint64 totalSeconds = duration / 1000;

    qint64 totalHours = totalSeconds / 3600;
    qint64 minutes = (totalSeconds % 3600) / 60;
    qint64 seconds = totalSeconds % 60;

    return QString("%1:%2:%3")
        .arg(totalHours, 2, 10, QChar(' '))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

QString TypeConversion::timeToString(long long time) const
{
    QDateTime dataTime;
    dataTime.setMSecsSinceEpoch(time);
    dataTime = dataTime.toUTC();
    return dataTime.toString("hh:mm:ss");
}

QString TypeConversion::intListToString(QList<int> list) const
{
    QStringList strList;
    for (int num : list) {
        strList.append(QString::number(num));
    }
    return "Int:" + strList.join(";");
}

QString TypeConversion::intListToString(QList<long long> list) const
{
    QStringList strList;
    for (long long num : list) {
        strList.append(QString::number(num));
    }
    return "Int:" + strList.join(";");
}

QString TypeConversion::stringListToString(QStringList list) const
{
    return "String:" + list.join(";");
}

QList<int> TypeConversion::stringToIntList(QString str) const
{
    if(!str.contains("Int:"))
        return QList<int> ();

    QStringList strList = str.split("Int:")[1].split(";");

    QList<int> list;
    for (int i = 0; i < strList.length(); i++) {
        list.append(strList[i].toInt());
    }
    return list;
}

QList<long long> TypeConversion::stringToLongList(QString str) const
{
    if(!str.contains("Int:"))
        return QList<long long> ();

    QStringList strList = str.split("Int:")[1].split(";");

    QList<long long> list;
    for (int i = 0; i < strList.length(); i++) {
        list.append(strList[i].toInt());
    }
    return list;
}

QList<QString> TypeConversion::stringToStringList(QString str) const
{
    if(!str.contains("String:"))
        return QList<QString> ();
    return str.split("String:")[1].split(";").toList();
}
