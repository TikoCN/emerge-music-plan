#ifndef EXTRALIBRARY_H
#define EXTRALIBRARY_H
#include "music.h"
#include <QAudioBuffer>
#include <QObject>

class ExtraLibrary
{
public:
    //设置文件参数
    bool setMedia(QString url, QString title, QString artist, QString alumb, QString genre, QString year);
    //得到文件参数
    void getMedia(Music* core);

    //从文件中提取图片
    QImage loadIndexCover(QString musicUrl);

   //将音乐文件转化为mp3
    int musicFileChangeMP3(QString url);

    //计算傅里叶变换
    QVector<double> useFftw3(QVector<double>in, int N);

};

#endif // EXTRALIBRARY_H
