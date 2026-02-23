#ifndef MUSIC_H
#define MUSIC_H

#include "mediadata.h"
#include <QObject>
#include <QFileInfo>

using MusicPtr = QSharedPointer<class Music>;

class Music {
public:
    QString title = QObject::tr("未知标题");
    QString artist = {QObject::tr("未知歌手")};
    QString album = {QObject::tr("未知专辑")};
    QString url = "";
    QString lastEdit = "";
    QString nameKey = "";
    long long duration = 0;
    long long lastEditTime = 0;
    long long insetTime = 0;
    int id = -1;
    int level = 0;
    int playNumber = 0;
    bool isLove = false;

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

#endif // MUSIC_H
