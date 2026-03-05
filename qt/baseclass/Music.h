#ifndef MUSIC_H
#define MUSIC_H

#include "MediaData.h"
#include "macro/ConstantProperty.h"
#include <QFileInfo>

#include "Album.h"

using MusicPtr = QSharedPointer<class Music>;

class Music {
    Q_GADGET
public:
    QT_CONSTANT_PROPERTY(QString, title, Title)
    QT_CONSTANT_PROPERTY(QString, artist, Artist)
    QT_CONSTANT_PROPERTY(QString, album, Album)
    QT_CONSTANT_PROPERTY(QString, url, Url)
    QT_CONSTANT_PROPERTY(QString, lastEdit, LastEdit)
    QT_CONSTANT_PROPERTY(QString, nameKey, NameKey)
    QT_CONSTANT_PROPERTY(long long, duration, Duration)
    QT_CONSTANT_PROPERTY(long long, lastEditTime, LastEditTime)
    QT_CONSTANT_PROPERTY(long long, insetTime, InsetTime)
    QT_CONSTANT_PROPERTY(int, id, Id)
    QT_CONSTANT_PROPERTY(int, level, Level)
    QT_CONSTANT_PROPERTY(int, playNumber, PlayNumber)
    QT_CONSTANT_PROPERTY(bool, isLove, IsLove)

    Music();

    // 读取元数据
    void setMedia(const MediaData &data);

    // 读取所有标签
    static QString getMediaJson();

    //来自文件
    void fromFileInfo(const QFileInfo &info);

    //加载封面
    static QImage loadCover(const QString &url);

    //加载封面·
    static QImage loadAloneCover(const QString &url);

    //判断是否符合搜索条件
    [[nodiscard]] bool isSearch(const QString &aim) const;

    //格式转换
    void setSuffix(const QString &type) const;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

Q_DECLARE_METATYPE(Music)
#endif // MUSIC_H
