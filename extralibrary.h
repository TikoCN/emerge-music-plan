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

    //计算傅里叶变换
    QVector<double> useFftw3(QVector<double>in, int N);

};

#endif // EXTRALIBRARY_H
