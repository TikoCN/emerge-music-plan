#ifndef MUSIC_H
#define MUSIC_H

#include "mediadata.h"
#include <QObject>
#include <QFileInfo>

using MusicPtr = QSharedPointer<class Music>;

class Music {
public:
    QString title;
    QStringList artistList;
    QString url;
    QString album;
    QString lastEdit;
    long long duration{};
    long long lastEditTime{};
    long long insetTime{};
    int id{};
    int level;
    int playNumber;
    int albumId{};
    bool isLove;

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

    [[nodiscard]] QString getArtist() const;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

#endif // MUSIC_H
