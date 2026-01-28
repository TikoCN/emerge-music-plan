#include "namekey.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
NameKey::NameKey(TLog *log)
: m_log(log) {
    const QDir dir(QDir::currentPath() + "/data/namekey");
    m_fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
} ;

QString NameKey::find(const QString& name) {
    m_findName = name.at(0);

    if (m_NameKeyHash.contains(m_findName)) {
        m_resultKey = m_NameKeyHash.value(m_findName);
    }
    else if (!readFileNameKey()) {
        m_resultKey = "未知";
    }

    return m_resultKey;
}

bool NameKey::readFileNameKey() {

    while (!m_fileInfoList.isEmpty()) {
        const auto fileUrl = m_fileInfoList.takeFirst().absoluteFilePath();
        m_log->logInfo(QObject::tr("开始在 %1 选找 NameKey").arg(fileUrl));
        QFile file(fileUrl);

        if (!file.open(QIODevice::ReadOnly))
            continue;

        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        const QJsonObject json = doc.object();

        QStringList keys = json.keys();
        for (const QString & key : keys) {
            if (key.contains(m_findName)) {
                m_NameKeyHash.insert(m_findName, key);
                m_resultKey = key;

                file.close();
                return true;
            }

            QStringList name = json.value(key).toString().split(",");
            for (const QString & j : name) {
                m_NameKeyHash.insert(j, key);
            }
        }

        file.close();
    }
    return  false;
}


