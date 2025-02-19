#ifndef EXTRALIBRARY_H
#define EXTRALIBRARY_H
#include "music.h"
#include <QAudioBuffer>
#include <QObject>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

struct formatConverByFFmpeg
{
    AVFormatContext *inFmtCtx = NULL;
    AVFormatContext *outFmtCtx = NULL;
    AVStream *inAudioStream = NULL;
    AVStream *outAudioStream = NULL;
    AVPacket *inPak = NULL;
    AVPacket *outPak = NULL;
    AVFrame *inFrame = NULL;
    AVFrame *outFrame = NULL;
    AVCodecContext *encodeCtx = NULL;
    AVCodecContext *decodeCtx = NULL;
    SwrContext *swr = NULL;
    int r = 0;

    //打印错误
    void logError(QString text);

    //转换为flac文件
    bool toFlac(QString url);

    //转换为mp3文件
    bool toMp3(QString url);

    //关闭所有
    void closeAll();
};

class ExtraLibrary
{
public:
    //设置文件参数
    bool setMedia(QString url, QString title, QString artist, QString alumb, QString genre, QString year);
    //得到文件参数
    void getMedia(Music* core);

    //从文件中提取图片
    QImage loadIndexCover(QString musicUrl);

    //计算傅里叶变换
    QVector<double> useFftw3(QVector<double>in, int N);

};

#endif // EXTRALIBRARY_H
