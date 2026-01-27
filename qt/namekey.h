#ifndef NAMEKEY_H
#define NAMEKEY_H
#include <QHash>
#include <QFileInfo>
class NameKey
{
public:
    NameKey();

    [[nodiscard]] QString find(const QString& name);

    bool readFileNameKey();
private:
    QHash<QString, QString> m_NameKeyHash;
    QString m_findName;
    QString m_resultKey;
    QFileInfoList m_fileInfoList;
};

#endif // NAMEKEY_H
