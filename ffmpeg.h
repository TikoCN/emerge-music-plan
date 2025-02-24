#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QDebug>
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

class FFmpeg : public QObject
{
public:
    ~FFmpeg();
    QList<AVFormatContext *>fmtCtxList;//输入输出上下文lieb
    QList<AVPacket *>pktList;//数据包指针
    QList<AVCodecContext *>codeCtxList;//编解码器指针列表
    QList<SwrContext *>swrList;//重采样指针列表
    QList<AVFrame *>frmList;//数据指针列表
    int r = 0;

    enum Suffix{MP3, FLAC, AAC, WMA, PCM16, PCM32, ALAC}suffix;

    //生成上下文
    int getAVFormatContext();

    //生成数据包
    int getAVPacket();

    //生成编解码
    int getAVCodecContext();

    //生成重采样指针
    int getSwrContext();

    //生成数据指针
    int getAVFrame();

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

    //设置编码器参数
    const AVCodec *setEncodeParmeters(AVCodecContext **encodec, AVCodecParameters *parameter);

    //加载变化输出帧
    QList<AVFrame *>changeFrame(AVFrame *swrFrm);
};

#endif // FFMPEG_H
