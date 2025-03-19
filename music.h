#ifndef MUSIC_H
#define MUSIC_H

#include "lrcdata.h"
#include "base/keyvaluepair.h"
#include <QObject>
#include <QFileInfo>

class Music : public QObject
{
    Q_OBJECT
public:
    QString title;
    QString artist;
    QString url;
    QString alumb;
    QString lastEdit;
    qint64 endTime;
    qint64 lastEditTime;
    int coreId;
    QPixmap *cover = NULL;

public:

    Music();

    //将数据写入文件
    Q_INVOKABLE void writeDataToFile(QStringList key, QStringList value);

    // 读取所有标签
    Q_INVOKABLE QList<KeyValuePair *> getAllKey();

    //来自文件
    void fromFileInfo(QFileInfo info);

    //获得封面路径
    QString getCoverUrl();

    Q_INVOKABLE QString getLrcUrl();

    //获得歌词文件路径
    QList<LrcData *> getLyricsData();

    //获得父文件夹
    QString getParentDir();

    //获得文件基础名字
    QString getBaseName();

    //获得时长
    Q_INVOKABLE QString getStringTime();

    //获得识别文本
    QString getKey();

    //加载封面
    QPixmap loadCover();

    //加载封面·
    QImage loadAloneCover();

    //判断是否符合搜索条件
    bool isSearch(QString aim);

    //得到lrc文件数据
    Q_INVOKABLE QString getLrcData();

    //获得搜索文本
    Q_INVOKABLE QString getSearchString();

    //复制音乐信息
    Q_INVOKABLE void copyMusicData();

    //复制音乐文件路径
    Q_INVOKABLE void copyMusicUrl();

    //打开音乐封面文件
    Q_INVOKABLE void openMusicDir();

    //打开音乐封面文件
    Q_INVOKABLE void openMusicCover();

    //打开音乐歌词文件
    Q_INVOKABLE void openMusicLrc();

    //格式转换
    Q_INVOKABLE void setSuffix(QString type);

    QString getTitle() const;

    QString getArtist() const;

    QString getUrl() const;

    QString getAlumb() const;

    QString getLastEdit() const;

    int getCoreId() const;

    QString getGenre() const;

    QString getYear() const;

private:
    Q_PROPERTY(QString title READ getTitle CONSTANT)
    Q_PROPERTY(QString artist READ getArtist CONSTANT)
    Q_PROPERTY(QString url READ getUrl CONSTANT)
    Q_PROPERTY(QString alumb READ getAlumb CONSTANT)
    Q_PROPERTY(QString lastEdit READ getLastEdit CONSTANT)
    Q_PROPERTY(int coreId READ getCoreId CONSTANT)
};

#endif // MUSIC_H
