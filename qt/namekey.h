#ifndef NAMEKEY_H
#define NAMEKEY_H
#include <QHash>
#include <QFileInfo>
#include "tlog.h"
class NameKey
{
public:
    NameKey(TLog *log);

    [[nodiscard]] QString find(const QString& name);

    bool readFileNameKey();
private:
    QHash<QString, QString> m_NameKeyHash;
    QString m_findName;
    QString m_resultKey;
    QFileInfoList m_fileInfoList;
    TLog *m_log;
};

#endif // NAMEKEY_H
