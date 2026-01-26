#include "namekey.h"
#include <QDir>
#include <QJsonObject>
NameKey::NameKey() {
    QFile file(QDir(QDir::currentPath()).filePath("namekey.json"));
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    const QJsonObject json = doc.object();
    QStringList keys = json.keys();

    for (const QString & key : keys) {
        QStringList name = json.value(key).toString().split(",");

        for (const QString & j : name) {
            m_NameKeyHash.insert(j, key);
        }
    }
    file.close();
}

QString NameKey::find(QString name) const {
    const long long size = name.size() >= 3 ? 3 : name.size();

    for (long long i = size; i > 0; --i) {
        if (QString s = name.left(i); m_NameKeyHash.contains(s))
            return m_NameKeyHash.value(s);
    }

    if (size > 0)
        return name[0];
    return "未知";
}


