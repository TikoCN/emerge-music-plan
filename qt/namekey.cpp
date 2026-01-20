#include "namekey.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
NameKey::NameKey() {
    QFile file(QDir(QDir::currentPath()).filePath("namekey.json"));
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject json = doc.object();
    QStringList keys = json.keys();

    for (int i = 0; i < keys.size(); ++i) {
        QStringList name = json.value(keys[i]).toString().split(",");

        for (int j = 0; j < name.size(); ++j) {
            m_NameKeyHash.insert(name[j], keys[i]);
        }
    }
    file.close();
}

QString NameKey::find(QString name)
{
    int size = name.size() >= 3 ? 3 : name.size();

    for (int i = size; i > 0; --i) {
        QString s = name.left(i);
        if (m_NameKeyHash.contains(s))
            return m_NameKeyHash.value(s);
    }

    if (size > 0)
        return name[0];
    return "未知";
}


