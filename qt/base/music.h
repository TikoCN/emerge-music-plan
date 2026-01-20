#ifndef MUSIC_H
#define MUSIC_H

#include "mediadata.h"
#include <QObject>
#include <QFileInfo>

using MusicPtr = QSharedPointer<class Music>;

class Music
{
public:
    QString title;
    QStringList artistList;
    QString url;
    QString album;
    QString lastEdit;
    long long duration;
    long long lastEditTime;
    long long insetTime;
    int id;
    int level;
    int playNumber;
    int albumId;
    bool isLove;

    Music();

    // 读取元数据
    void setMedia(MediaData data);

    // 读取所有标签
    QString getMediaJson();

    //来自文件
    void fromFileInfo(QFileInfo info);

    //加载封面
    static QImage loadCover(QString url);

    //加载封面·
    static QImage loadAloneCover(QString url);

    //判断是否符合搜索条件
    bool isSearch(QString aim);

    //格式转换
    void setSuffix(QString type);

    QString getArtist() const;
    QJsonObject getJsonObject();
};

#endif // MUSIC_H
