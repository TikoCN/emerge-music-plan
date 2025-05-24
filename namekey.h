#ifndef NAMEKEY_H
#define NAMEKEY_H
#include <QHash>
class NameKey
{
public:
    NameKey();

    QString find(QString name);
private:
    QHash<QString, QString> m_NameKeyHash;
};

#endif // NAMEKEY_H
