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

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    //下载封面
    Q_INVOKABLE void downCover(QString key, QString url);

    Q_INVOKABLE void downCoverFromNetEase(QString key, QString url);

    Q_INVOKABLE void downCoverFromQQMusic(QString key, QString url);

    Q_INVOKABLE void downCoverFromBing(QString key, QString url);

    Q_INVOKABLE void downCoverFromBaidu(QString key, QString url);

    //加载图片写入文件
    void writeCoverToFile(QString url, QString file);

    //下载歌词
    Q_INVOKABLE void downLrc(QString key, QString url);

    Q_INVOKABLE void downLrcFromNetEase(QString key, QString url);

    Q_INVOKABLE void downLrcFromQQMusic(QString key, QString url);

signals:
    void lrcDowned();
    void coverDowned();
};

#endif // ONLINE_H
