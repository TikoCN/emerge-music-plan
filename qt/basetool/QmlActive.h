#ifndef QMLACTIVE_H
#define QMLACTIVE_H

#include <QObject>

class QmlActive : public QObject
{
    Q_OBJECT
public:
    explicit QmlActive(QObject *parent = nullptr);

signals:
};

#endif // QMLACTIVE_H
