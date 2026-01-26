#ifndef FFMPEG_H
#define FFMPEG_H

#include <QDebug>
#include "baseclass/mediadata.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

class FFmpeg : public QObject
{
public:
    int m_r = 0;

    enum Suffix{MP3, FLAC, AAC, WMA, PCM16, PCM32, ALAC}suffix;

    static QString suffixToString(Suffix s);

    //打开输出文件
    AVFormatContext *getOutFormatContext(const QString& url);

    //打开输入文件
    AVFormatContext *getInputFormatContext(const QString& url);

    //得到解码器文本
    AVCodecContext *getDecodecContext(AVCodecID id, const AVCodecParameters *para);

    //得到编码器文本
    AVCodecContext *getEncodecContext(AVCodecID id, const AVCodecParameters *para, bool defaultCodeFlag);

    //生成数据包
    static AVPacket *getAVPacket();

    //生成重采样指针
    static SwrContext *getSwrContext(const AVCodecContext *out, const AVCodecContext *in);

    //生成数据指针
    static AVFrame *getAVFrame();

    //打印错误
    void logError(const QString &text) const;

    //从音乐文件中提取独立封面
    QImage getInlayCover(const QString& url);

    //将音乐文件内嵌的音乐文件中
    void setInlayCover(const QString& musicUrl, const QString& coverUrl);

    //得到输出路径
    [[nodiscard]] QString getOutUrl(QString inUrl) const;

    //转码装格式
    bool transformCodec(const QString& url, enum Suffix);

    //加载变化输出帧
    QList<AVFrame *>changeFrame(AVFrame *swrFrm) const;

    //写入标签
    bool writeDict(QStringList key, QStringList value, const QString& inUrl, const QString& outUrl);

    //得到标签
    bool getDict(QStringList *keys, QStringList *values, const QString& url);
    bool getDict(MediaData *data, const QString& url);
};

#endif // FFMPEG_H
