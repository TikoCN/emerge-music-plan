#ifndef TYPECONVERSION_H
#define TYPECONVERSION_H

#include <QObject>

class TypeConversion : public QObject
{
    Q_OBJECT
public:
    explicit TypeConversion(QObject *parent = nullptr);

    Q_INVOKABLE static QString durationToTimeStringNoMax(long long duration) ;
    Q_INVOKABLE static QString timeToString(long long time) ;

    static QString intListToString(const QList<int>& list);

    static QString intListToString(const QList<long long>& list);
    static QString stringListToString(const QStringList& list) ;

    Q_INVOKABLE static QList<int> stringToIntList(const QString& str);
    Q_INVOKABLE static QList<long long> stringToLongList(const QString& str);
    Q_INVOKABLE static QList<QString> stringToStringList(const QString &str) ;
signals:
};

#endif // TYPECONVERSION_H
