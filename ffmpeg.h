#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QDebug>
#include "base/mediadata.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

class FFmpeg : public QObject
{
public:
    int r = 0;

    enum Suffix{MP3, FLAC, AAC, WMA, PCM16, PCM32, ALAC}suffix;

    QString suffixToString(Suffix s);

    //打开输出文件
    AVFormatContext *getOutFormatContext(QString url);

    //打开输入文件
    AVFormatContext *getInputFormatContext(QString url);

    //得到解码器文本
    AVCodecContext *getDecodecContext(AVCodecID id, AVCodecParameters *para);

    //得到编码器文本
    AVCodecContext *getEncodecContext(AVCodecID id, AVCodecParameters *para, bool defult);

    //生成数据包
    AVPacket *getAVPacket();

    //生成重采样指针
    SwrContext *getSwrContext(AVCodecContext *out, AVCodecContext *in);

    //生成数据指针
    AVFrame *getAVFrame();

    //打印错误
    void logError(QString text);

    //从音乐文件中提取独立封面
    QImage getInlayCover(QString url);

    //将音乐文件内嵌的音乐文件中
    void setInlayCover(QString musicUrl, QString coverUrl);

    //得到输出路径
    QString getOutUrl(QString inUrl);

    //转码装格式
    bool transformCodec(QString url, enum Suffix);

    //加载变化输出帧
    QList<AVFrame *>changeFrame(AVFrame *swrFrm);

    //写入标签
    bool writeDict(QStringList key, QStringList value, QString inUrl, QString outUrl);

    //得到标签
    bool getDict(QStringList *keys, QStringList *values, QString url);
    bool getDict(MediaData *data, QString url);
};

#endif // FFMPEG_H
