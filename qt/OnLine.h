#ifndef ONLINE_H
#define ONLINE_H

#include <QObject>

class OnLine : public QObject
{
    Q_OBJECT

private:
    static OnLine* instance;
    OnLine();

public:
    static OnLine* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new OnLine;
        }
    }

    static void freeInstance() {
            delete instance;
    }

    //下载封面
    Q_INVOKABLE static void downMusicCover(const QString& key, const QString& url);
    Q_INVOKABLE static void downArtistCover(const QString& key, const QString& url);
    Q_INVOKABLE static void downAlbumCover(const QString& key, const QString& url);

    Q_INVOKABLE static void downMusicCoverNetEase(const QString& key, const QString &url);

    Q_INVOKABLE static void downMusicCoverQQMusic(const QString& key, const QString &url);

    Q_INVOKABLE static void downCoverBing(const QString &key, const QString &url);

    Q_INVOKABLE static void downCoverBaidu(const QString& key, const QString &url);

    //加载图片写入文件
    static void writeCoverToFile(const QString& url, const QString& file);

    //下载歌词
    Q_INVOKABLE void downLrc(const QString& key, const QString &url, int musicId);

    Q_INVOKABLE static void downLrcFromNetEase(const QString& key, const QString &url);

    Q_INVOKABLE static void downLrcFromQQMusic(const QString& key, const QString &url);

    // 下载音乐元数据
    Q_INVOKABLE static void downMediaFromNetEase(const QString& search, const QString& musicUrl);

    Q_INVOKABLE static void downMediaFromQQMusic(const QString& search, const QString& musicUrl);

signals:
    void lrcDowned(int musicId);
};

#endif // ONLINE_H
