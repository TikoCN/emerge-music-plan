#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>
#include <QFileInfo>

class Music : public QObject//音乐单元
{
    Q_OBJECT
public:
    QString title;
    QString artist;
    QString url;
    QString alumb;
    QString lastEdit;
    QString genre;
    QString year;
    qint64 endTime;
    double maxHeight;
    double minHeight;
    int coreId;
    int playNumber;
    bool love;
    QPixmap *cover = NULL;

public:

    Music();

    //来自文件
    void fromFileInfo(QFileInfo info);

    //来自Json
    void fromJson(QJsonObject obj);

    //获得封面路径
    QString getCoverUrl();

    //获得歌词文件路径
    QString getLrcUrl();

    //获得父文件夹
    QString getParentDir();

    //获得时长
    Q_INVOKABLE QString getStringTime();

    //获得最后编辑时间
    qint64 getNumberEdit();

    //获得识别文本
    QString getKey();

    //加载封面
    QPixmap loadCover();

    //加载封面
    QImage loadAloneCover();

    //加载封面网络
    QPixmap loadCoverOnline();

    //判断是否符合搜索条件
    bool isSearch(QString aim);

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

    QString getTitle() const;
    void setTitle(const QString &newTitle);

    QString getArtist() const;
    void setArtist(const QString &newArtist);

    QString getUrl() const;
    void setUrl(const QString &newUrl);

    QString getAlumb() const;
    void setAlumb(const QString &newAlumb);

    QString getLastEdit() const;
    void setLastEdit(const QString &newLastEdit);

    int getCoreId() const;
    void setCoreId(int newCoreId);

    bool getLove() const;
    void setLove(bool newLove);

    QString getGenre() const;
    void setGenre(const QString &newGenre);

    QString getYear() const;
    void setYear(const QString &newYear);

    int getPlayNumber() const;
    void setPlayNumber(int newPlayNumber);

signals:
    void titleChanged();

    void artistChanged();

    void urlChanged();

    void alumbChanged();

    void lastEditChanged();

    void coreIdChanged();

    void loveChanged();

    void genreChanged();

    void yearChanged();

    void playNumberChanged();

private:
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString artist READ getArtist WRITE setArtist NOTIFY artistChanged FINAL)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(QString alumb READ getAlumb WRITE setAlumb NOTIFY alumbChanged FINAL)
    Q_PROPERTY(QString lastEdit READ getLastEdit WRITE setLastEdit NOTIFY lastEditChanged FINAL)
    Q_PROPERTY(int coreId READ getCoreId WRITE setCoreId NOTIFY coreIdChanged FINAL)
    Q_PROPERTY(bool love READ getLove WRITE setLove NOTIFY loveChanged FINAL)
    Q_PROPERTY(QString genre READ getGenre WRITE setGenre NOTIFY genreChanged FINAL)
    Q_PROPERTY(QString year READ getYear WRITE setYear NOTIFY yearChanged FINAL)
    Q_PROPERTY(int playNumber READ getPlayNumber WRITE setPlayNumber NOTIFY playNumberChanged FINAL)
};

#endif // MUSIC_H
