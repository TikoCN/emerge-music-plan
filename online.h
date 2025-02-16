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

    //下载封面
    void downCover(QString key, QString url);

    void downCoverFromNetEase(QString key, QString url);

    void downCoverFromQQMusic(QString key, QString url);

    void downCoverFromBing(QString key, QString url);

    void downCoverFromBaidu(QString key, QString url);

    //加载图片写入文件
    void writeCoverToFile(QString url, QString file);

    //下载歌词
    void downLrc(QString key, QString url);

    void downLrcFromNetEase(QString key, QString url);

    void downLrcFromQQMusic(QString key, QString url);

signals:
    void lrcDowned();
    void coverDowned();
};

#endif // ONLINE_H
