#ifndef MUSIC_H
#define MUSIC_H

#include "lrcdata.h"
#include "base/keyvaluepair.h"
#include "mediadata.h"
#include <QObject>
#include <QFileInfo>

class Music : public QObject
{
    Q_OBJECT
public:
    QString title;
    QStringList artistList;
    QString url;
    QString album;
    QString lastEdit;
    long long endTime;
    long long lastEditTime;
    long long insetTime;
    int coreId;
    int level;
    int playNumber;
    int albumId;
    bool isLove;

    Music();

    //将数据写入文件
    Q_INVOKABLE void writeDataToFile(QStringList key, QStringList value);

    // 读取元数据
    void readMedia();
    void setMedia(MediaData data);

    // 读取所有标签
    Q_INVOKABLE QList<KeyValuePair *> getAllKey();

    //来自文件
    void fromFileInfo(QFileInfo info);

    Q_INVOKABLE QString getLrcUrl();

    //获得歌词文件路径
    QList<LrcData *> getLyricsData();

    //获得时长
    Q_INVOKABLE QString getStringTime();

    //加载封面
    static QImage loadCover(QString url);

    //加载封面·
    static QImage loadAloneCover(QString url);

    //判断是否符合搜索条件
    bool isSearch(QString aim);

    //复制音乐信息
    Q_INVOKABLE void getString();

    //格式转换
    Q_INVOKABLE void setSuffix(QString type);

    QString getTitle() const;

    QString getArtist() const;

    QString getUrl() const;

    QString getAlbum() const;

    QString getLastEdit() const;

    int getCoreId() const;

    QString getGenre() const;

    QString getYear() const;

    bool getIsLove() const;
    void setIsLove(bool newIsLove);

    int getLevel() const;
    void setLevel(int newLevel);

    long long getEndTime() const;

    int getPlayNumber() const;
    void setPlayNumber(int newPlayNumber);

signals:
    void isLoveChanged();

    void levelChanged();

    void playNumberChanged();

private:
    Q_PROPERTY(QString title READ getTitle CONSTANT)
    Q_PROPERTY(QString artist READ getArtist CONSTANT)
    Q_PROPERTY(QString url READ getUrl CONSTANT)
    Q_PROPERTY(QString album READ getAlbum CONSTANT)
    Q_PROPERTY(QString lastEdit READ getLastEdit CONSTANT)
    Q_PROPERTY(int coreId READ getCoreId CONSTANT)
    Q_PROPERTY(bool isLove READ getIsLove WRITE setIsLove NOTIFY isLoveChanged FINAL)
    Q_PROPERTY(int level READ getLevel WRITE setLevel NOTIFY levelChanged FINAL)
    Q_PROPERTY(long long endTime READ getEndTime CONSTANT FINAL)
    Q_PROPERTY(int playNumber READ getPlayNumber WRITE setPlayNumber NOTIFY playNumberChanged FINAL)
};

#endif // MUSIC_H
