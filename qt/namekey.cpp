#include "namekey.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
NameKey::NameKey() {
    const QDir dir(QDir(QDir::currentPath()).filePath("namekey"));
    m_fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
};

QString NameKey::find(const QString& name) {
    m_findName = name;
    const int size = name.size() >= 3 ? 3 : static_cast<int>(name.size());

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
        QFile file(m_fileInfoList.takeFirst().absolutePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;

        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        const QJsonObject json = doc.object();

        QStringList keys = json.keys();
        for (const QString & key : keys) {
            if (keys.contains(m_findName)) {
                m_NameKeyHash.insert(m_findName, key);
                m_resultKey = key;

                file.close();
                return true;
            }

            for (QStringList name = json.value(key).toString().split(","); const QString & j : name) {
                m_NameKeyHash.insert(j, key);
            }
        }

        file.close();
    }
    return  false;
}


