#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

#include <QObject>

class KeyValuePair : public QObject
{
    Q_OBJECT
public:
    explicit KeyValuePair(QString key, QString value, QObject *parent = nullptr);

    QString key;
    QString value;

    QString getValue() const;
    QString getKey() const;

signals:
    void valueChanged();
    void keyChanged();

private:
    Q_PROPERTY(QString value READ getValue CONSTANT)
    Q_PROPERTY(QString key READ getKey CONSTANT)
};

#endif // KEYVALUEPAIR_H
