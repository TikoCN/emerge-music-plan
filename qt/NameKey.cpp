#include "NameKey.h"

#include <QCoreApplication>
#include <cpp-pinyin/G2pglobal.h>

#include <filesystem>
#include <memory>
#include <string>

static const Pinyin::Pinyin &pinyinEngine() {
    static const std::unique_ptr<Pinyin::Pinyin> engine = [] {
        const QString dictionaryPath = QCoreApplication::applicationDirPath() + "/dict";
        Pinyin::setDictionaryPath(std::filesystem::path(dictionaryPath.toStdWString()));
        return std::make_unique<Pinyin::Pinyin>();
    }();
    return *engine;
}

NameKey::NameKey()
= default;

QString NameKey::find(const QString &name) {
    if (name.isEmpty()) {
        return QStringLiteral("未知");
    }

    const std::string utf8Name = name.toUtf8().toStdString();
    const Pinyin::PinyinResVector pinyinList = pinyinEngine().hanziToPinyin(utf8Name);
    if (pinyinList.empty()) {
        return QStringLiteral("未知");
    }

    const std::string &firstPinyin = pinyinList.front().pinyin;
    for (const char character: firstPinyin) {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')) {
            return QString(QChar(character).toUpper());
        }
    }

    return QStringLiteral("未知");
}
