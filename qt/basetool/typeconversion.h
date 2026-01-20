#ifndef TYPECONVERSION_H
#define TYPECONVERSION_H

#include <QObject>

class TypeConversion : public QObject
{
    Q_OBJECT
public:
    explicit TypeConversion(QObject *parent = nullptr);

    Q_INVOKABLE QString durationToTimeStringNoMax(long long duration);
    Q_INVOKABLE QString timeToString(long long time);

    QString intListToString(QList<int> list);
    QString intListToString(QList<long long> list);
    QString stringListToString(QStringList list);

    Q_INVOKABLE QList<int> stringToIntList(QString str);
    Q_INVOKABLE QList<long long> stringToLongList(QString str);
    Q_INVOKABLE QList<QString> stringToStringList(QString str);
signals:
};

#endif // TYPECONVERSION_H
