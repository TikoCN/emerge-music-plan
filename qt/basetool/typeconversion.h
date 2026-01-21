#ifndef TYPECONVERSION_H
#define TYPECONVERSION_H

#include <QObject>

class TypeConversion : public QObject
{
    Q_OBJECT
public:
    explicit TypeConversion(QObject *parent = nullptr);

    Q_INVOKABLE QString durationToTimeStringNoMax(long long duration) const;
    Q_INVOKABLE QString timeToString(long long time) const;

    QString intListToString(QList<int> list) const;
    QString intListToString(QList<long long> list) const;
    QString stringListToString(QStringList list) const;

    Q_INVOKABLE QList<int> stringToIntList(QString str) const;
    Q_INVOKABLE QList<long long> stringToLongList(QString str) const;
    Q_INVOKABLE QList<QString> stringToStringList(QString str) const;
signals:
};

#endif // TYPECONVERSION_H
