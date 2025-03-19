#include "ffmpeg.h"
#include <QImage>
#include <QFile>

FFmpeg::~FFmpeg()
{
    //关闭输入输出上下文
    while(fmtCtxList.size() > 0){
        //如果打开，关闭输出文件
        if (fmtCtxList.last()->oformat != nullptr && !(fmtCtxList.last()->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&fmtCtxList.last()->pb);
        }
        avformat_free_context(fmtCtxList.last());

        fmtCtxList.removeLast();
    }

    //释放数据包指针
    while(pktList.size() > 0){
        av_packet_unref(pktList.last());
        av_packet_free(&pktList.last());
        pktList.removeLast();
    }
    while(frmList.size() > 0){
        av_frame_unref(frmList.last());
        av_frame_free(&frmList.last());
        frmList.removeLast();
    }

    //编解码器指针列表
    while(codeCtxList.size() > 0){
        avcodec_free_context(&codeCtxList.last());
        codeCtxList.removeLast();
    }

    //释放重采样列表
    while(swrList.size() > 0){
        swr_free(&swrList.last());
        swrList.removeLast();
    }
}

QString FFmpeg::suffixToString(Suffix s)
{
    switch (s) {
    case MP3: return "mp3";
    case FLAC: return "flac";
    case AAC: return "aac";
    case ALAC: return "alac";
    case WMA: return "wma";
    default: return "none";
    }

    return "none";
}

/*
 * 得到输出上下文
 */
AVFormatContext *FFmpeg::getOutFormatContext(QString url)
{
    AVFormatContext *fmtCtx = nullptr;
    //打开输出文件
    r = avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, url.toUtf8());
    if(r<0){
        throw QString(url + "output file open fail");
    }

    fmtCtxList.append(fmtCtx);
    return fmtCtx;
}

/*
 * 得到输入上下文
 */
AVFormatContext *FFmpeg::getInputFormatContext(QString url)
{
    AVFormatContext *fmtCtx = nullptr;
    //打开输出文件
    r = avformat_open_input(&fmtCtx, url.toUtf8(), nullptr, nullptr);
    if(r<0){
        throw QString(url + "open input file fail");
    }

    //查找流信息
    r = avformat_find_stream_info(fmtCtx, nullptr);
    if(r<0){
        throw QString(url + "no stream find");
    }

    fmtCtxList.append(fmtCtx);
    return fmtCtx;
}

/*
 * 得到解码器
 */
AVCodecContext *FFmpeg::getDecodecContext(AVCodecID id, AVCodecParameters *para)
{
    //寻找编解码器
    const AVCodec *codec = avcodec_find_encoder(id);
    if(!codec){
        throw QString("no find codec");
    }
    QString name = QString::fromUtf8(codec->name);

    //生成编解码上下文
    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx){
        throw QString(name + "no find codec");
    }

    r = avcodec_parameters_to_context(codecCtx, para);
    if(r<0){
        throw QString(name + "copy parameter to context fial");
    }

    r = avcodec_open2(codecCtx, codec, nullptr);
    if(r < 0){
        throw QString(name + " decodecContext and encodec open fail");
    }

    //插入队列并返回
    codeCtxList.append(codecCtx);
    return codecCtx;
}

/*
 * 得到编码器
 */
AVCodecContext *FFmpeg::getEncodecContext(AVCodecID id, AVCodecParameters *para, bool defult)
{
    //选择默认编码器类型
    if(defult){
        switch (suffix) {
        case MP3:
            id = AV_CODEC_ID_MP3;
            break;
        case FLAC:
            id = AV_CODEC_ID_FLAC;
            break;
        case AAC:
            id = AV_CODEC_ID_AAC;
            break;
        case ALAC:
            id = AV_CODEC_ID_ALAC;
            break;
        case PCM16:
            id = AV_CODEC_ID_PCM_S16LE;
            break;
        case PCM32:
            id = AV_CODEC_ID_PCM_S32LE;
            break;
        case WMA:
            id = AV_CODEC_ID_WMAV2;
            break;
        default:
            break;
        }
    }

    //寻找编解码器
    const AVCodec *codec = avcodec_find_encoder(id);
    if(!codec){
        throw QString(suffixToString(suffix) + "no find codec");
    }

    //生成编解码上下文
    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx){
        throw QString(suffixToString(suffix) + "no find codec");
    }

    r = avcodec_parameters_to_context(codecCtx, para);
    if(r<0){
        throw QString(suffixToString(suffix) + "copy parameter to context fial");
    }

    //设置编码器默认参数
    switch (id) {
    case AV_CODEC_ID_MP3:
        codecCtx->codec_id = AV_CODEC_ID_MP3;
        codecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        codecCtx->sample_fmt = AV_SAMPLE_FMT_S32P;
        codecCtx->sample_rate = 44100;
        codecCtx->bit_rate = 320000;
        break;
    case AV_CODEC_ID_FLAC:
        codecCtx->codec_id = AV_CODEC_ID_FLAC;
        codecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        codecCtx->sample_fmt = AV_SAMPLE_FMT_S16;
        codecCtx->sample_rate = 44100;
        codecCtx->bit_rate = 0;
        codecCtx->time_base.den = codecCtx->sample_rate;
        codecCtx->time_base.num = 1;
        codecCtx->bits_per_raw_sample = 16;
        break;
    case AV_CODEC_ID_AAC:
        codecCtx->codec_id = AV_CODEC_ID_AAC;
        codecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        codecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
        codecCtx->sample_rate = 44100;
        codecCtx->bit_rate = 320000;
        break;
    case AV_CODEC_ID_ALAC:
        codecCtx->codec_id = AV_CODEC_ID_ALAC;
        codecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        codecCtx->sample_fmt = AV_SAMPLE_FMT_S16P;
        codecCtx->sample_rate = 44100;
        codecCtx->bit_rate = 320000;
        break;
    case AV_CODEC_ID_WMAV2:
        codecCtx->codec_id = AV_CODEC_ID_WMAV2;
        codecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        codecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
        codecCtx->sample_rate = 44100;
        codecCtx->bit_rate = 320000;
        break;
    default:
        break;
    }

    r = avcodec_open2(codecCtx, codec, nullptr);
    if(r < 0){
        throw QString(QString::fromUtf8(codec->name) + " encodecContext and codec open fail");
    }

    //插入队列并返回
    codeCtxList.append(codecCtx);
    return codecCtx;
}

int FFmpeg::getAVPacket()
{
    AVPacket *pkt = nullptr;
    pkt = av_packet_alloc();
    pktList.append(pkt);
    return pktList.size() - 1;
}

SwrContext *FFmpeg::getSwrContext(AVCodecContext *out, AVCodecContext *in)
{
    SwrContext *swr = nullptr;
    swr_alloc_set_opts2(&swr,
                        &out->ch_layout,
                        out->sample_fmt,
                        out->sample_rate,
                        &in->ch_layout,
                        in->sample_fmt,
                        in->sample_rate,
                        0, nullptr);

    swrList.append(swr);
    return swr;
}

int FFmpeg::getAVFrame()
{
    AVFrame *frm = nullptr;
    frm = av_frame_alloc();
    frmList.append(frm);
    return frmList.size() - 1;
}

/*
 * 打印错误日志
 */
void FFmpeg::logError(QString text){
    char error[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(r, error, AV_ERROR_MAX_STRING_SIZE);
    qDebug() << "ffmpeg error number : " + QString::number(r) + " = error, working " + text;
}

/*
 * 从音乐文件中提取独立封面
 */
QImage FFmpeg::getInlayCover(QString url)
{

    QImage img;
    try{
        //从文件中读取封面
        AVFormatContext *fmt_ctx = getInputFormatContext(url);

        for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++){
            AVStream *stream = fmt_ctx->streams[i];
            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && stream->disposition & AV_DISPOSITION_ATTACHED_PIC){
                AVPacket pkt = stream->attached_pic;
                //使用QImage读取完整图片数据
                img = QImage::fromData((uchar*)pkt.data, pkt.size);
            }
        }
    }
    catch(QString e){
        logError(e);
    }

    return img;
}

/*
 * 将音乐文件内嵌的音乐文件中
 */
void FFmpeg::setInlayCover(QString musicUrl, QString coverUrl)
{
    try{
        QString outUrl = musicUrl;
        //设置内嵌封面之后的路径
        outUrl.replace("." + outUrl.split(".").last(), "-New." + outUrl.split(".").last());

        //探测音乐文件，和封面文件是否存在
        if(!QFile::exists(musicUrl)){
            throw QString("no find music file");
        }
        if(!QFile::exists(coverUrl)){
            throw QString("no find cover file");
        }

        //读取封面中的数据
        QFile cover(coverUrl);
        if(!cover.open(QIODevice::ReadOnly)){
            cover.close();
            throw QString("open cover file fail");
        }
        QByteArray data = cover.readAll();
        cover.close();

        AVFormatContext *inFmtCtx = getInputFormatContext(musicUrl);
        AVFormatContext *outFmtCtx = getOutFormatContext(outUrl);
        AVStream *inStream = nullptr;
        AVStream *outStream = nullptr;
        int inPak = getAVPacket();

        int aimStream = -1;
        for(int i=0; i<inFmtCtx->nb_streams; i++){
            inStream = inFmtCtx->streams[i];
            outStream = avformat_new_stream(outFmtCtx, nullptr);
            avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
            if(inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && inStream->disposition & AV_DISPOSITION_ATTACHED_PIC){
                aimStream = i;
            }
        }

        //创建一条命封面流
        if(aimStream == -1){
            outStream = avformat_new_stream(outFmtCtx, nullptr);
            outStream->codecpar->codec_id = AV_CODEC_ID_MJPEG;
            outStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
            outStream->disposition |= AV_DISPOSITION_ATTACHED_PIC;
            outStream->codecpar->width = 300;
            outStream->codecpar->height = 300;
            aimStream = outStream->index;
        }

        av_dict_copy(&outFmtCtx->metadata, inFmtCtx->metadata, 0);

        r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
        if(r<0){
            throw QString(outUrl + " pb open fail");
        }

        //写入文件头
        r = avformat_write_header(outFmtCtx, nullptr);
        if(r<0){
            throw QString(outUrl + " write header fail");
        }

        pktList[inPak] = av_packet_alloc();
        while(av_read_frame(inFmtCtx, pktList[inPak]) >= 0){
            if(pktList[inPak]->stream_index == aimStream){
                //放弃旧数据
                continue;
            }
            av_interleaved_write_frame(outFmtCtx, pktList[inPak]);
        }

        //写入封面包
        av_packet_unref(pktList[inPak]);
        av_new_packet(pktList[inPak], data.size());

        //拷贝数据包
        memcpy(pktList[inPak]->data, data.data(), data.size());
        pktList[inPak]->stream_index = aimStream;
        pktList[inPak]->pts = 0;
        pktList[inPak]->duration = inFmtCtx->duration;
        av_interleaved_write_frame(outFmtCtx, pktList[inPak]);

        //写入文件尾
        r = av_write_trailer(outFmtCtx);
        if(r<0){
            throw QString(outUrl + " write trailer fail");
        }
    }
    catch(QString e){
        logError(e + "inlay cover write fail");
        return;
    }
}

/*
 * 得到输出文件路径
 */
QString FFmpeg::getOutUrl(QString inUrl)
{
    QString inSuffix = inUrl.split(".").last();
    QString aim = "mp3";
    switch (suffix) {
    case MP3:
        aim = "mp3";
        break;
    case FLAC:
        aim = "flac";
        break;
    case AAC:
        aim = "aac";
        break;
    case ALAC:
        aim = "m4a";
        break;
    case PCM16:
        aim = "pcm";
        break;
    case PCM32:
        aim = "pcm";
        break;
    case WMA:
        aim = "wma";
        break;
    default:
        break;
    }
    QString outUrl;
    if(inSuffix == aim){
        outUrl = inUrl.replace("."+inSuffix, "-New."+aim);
    }
    else{
        outUrl = inUrl.replace("."+inSuffix, "."+aim);
    }
    return outUrl;
}

/*
 * 对输入文件进行转码
 */
bool FFmpeg::transformCodec(QString url, Suffix aim)
{
    suffix = aim;
    QString inUrl = url;
    QString outUrl = getOutUrl(url);

    try{
        AVFormatContext *inFmtCtx = getInputFormatContext(inUrl);//输入上下文
        AVFormatContext *outFmtCtx = getOutFormatContext(outUrl);//输出上下文
        QVector<SwrContext *>reSwrList;//重采样列表
        QVector<AVCodecContext *>deList;//解码器列表
        QVector<AVCodecContext *>enList;//编码器列表
        QVector<int>indexList;//流转换列表
        int inFrm = getAVFrame();
        int swrFrm = getAVFrame();
        int inPkt = getAVPacket();
        int outPkt = getAVPacket();

        //分配字长
        deList.fill(nullptr, inFmtCtx->nb_streams);
        enList.fill(nullptr, inFmtCtx->nb_streams);
        reSwrList.fill(nullptr, inFmtCtx->nb_streams);
        indexList.fill(-1, inFmtCtx->nb_streams);

        //遍历流生成编解码器
        int n = 0;
        for(int i=0; i<inFmtCtx->nb_streams; i++){
            AVStream *inStream = inFmtCtx->streams[i];

            //复制附加封面流
            if(suffix != AAC &&
                suffix != ALAC &&
                inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
                inStream->disposition & AV_DISPOSITION_ATTACHED_PIC){
                AVStream * outStream = avformat_new_stream(outFmtCtx, nullptr);
                avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
                //写入流序号转换
                indexList[i] = n;
                n++;
            }

            //保留为音频流申请编解码器
            if(inStream->codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
                continue;
            }
            //写入流序号转换
            indexList[i] = n;
            n++;
            //生成音频流
            AVStream * outStream = avformat_new_stream(outFmtCtx, nullptr);
            if(outStream == nullptr){
                throw QString("get new stream fail");
            }
            avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);           //复制流

            //生成解码器
            AVCodecContext *decodeCtx = getDecodecContext(inStream->codecpar->codec_id, inStream->codecpar);
            deList[i] = decodeCtx; //添加的解码器列表

            //建立编码器
            AVCodecContext *encodeCtx = getEncodecContext(outStream->codecpar->codec_id, inStream->codecpar, true);
            enList[i] = encodeCtx;                                                       //添加到编码器列表

            //申请重采样
            SwrContext *swr = getSwrContext(encodeCtx, decodeCtx);
            reSwrList[i] = swr;
        }

        //拷贝元数据
        av_dict_copy(&outFmtCtx->metadata, inFmtCtx->metadata, 0);

        if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
            r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
            if (r < 0) {
                throw QString("Could not open output file");
            }
        }

        r = avformat_write_header(outFmtCtx, nullptr);
        if(r<0){
            throw QString("write header fail");
        }

        //读取数据设置循环解码
        int64_t pts = 0;
        while(av_read_frame(inFmtCtx, pktList[inPkt]) >= 0){
            int index = pktList[inPkt]->stream_index;
            //判断是否保留，不是则跳过
            if(indexList[index] == -1){
                av_packet_unref(pktList[inPkt]);//释放输入
                continue;
            }

            //并非音频流，直接写入
            if(inFmtCtx->streams[index]->codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
                r = av_interleaved_write_frame(outFmtCtx, pktList[inPkt]);
                if(r<0){
                    logError("write frame fail");
                }
                av_packet_unref(pktList[inPkt]);//释放输入
                continue;
            }

            //音频流进行解码，重采样，编码
            AVCodecContext *decodeCtx = deList[index];
            SwrContext *swr = reSwrList[index];
            AVCodecContext *encodeCtx = enList[index];
            //解码数据
            r = avcodec_send_packet(decodeCtx, pktList[inPkt]);
            if(r<0){
                logError("send packet fail");
            }

            //获得解码之后的数据
            while(avcodec_receive_frame(decodeCtx, frmList[inFrm]) >= 0){
                frmList[swrFrm]->sample_rate = encodeCtx->sample_rate;
                frmList[swrFrm]->format = encodeCtx->sample_fmt;
                frmList[swrFrm]->ch_layout = encodeCtx->ch_layout;

                //重采样
                r = swr_convert_frame(swr, frmList[swrFrm], frmList[inFrm]);
                if(r<0){
                    logError("swr fail");
                }

                QList<AVFrame *>outFrmList = changeFrame(frmList[swrFrm]);
                for(int i=0; i<outFrmList.size(); i++){
                    //设置帧样本大小
                    //encodeCtx->frame_size = outFrmList[i]->nb_samples;

                    r = avcodec_send_frame(encodeCtx, outFrmList[i]);
                    if(r<0){
                        logError("send frame fail");
                        av_frame_unref(outFrmList[i]);
                        av_frame_free(&outFrmList[i]);
                        continue;
                    }

                    while(avcodec_receive_packet(encodeCtx, pktList[outPkt]) >= 0){
                        r = av_interleaved_write_frame(outFmtCtx, pktList[outPkt]);
                        if(r<0){
                            logError("write frame fail");
                        }
                        av_packet_unref(pktList[outPkt]);
                    }
                    av_frame_unref(outFrmList[i]);
                    av_frame_free(&outFrmList[i]);
                    av_frame_unref(frmList[swrFrm]);
                }
                av_frame_unref(frmList[inFrm]);
            }
            av_packet_unref(pktList[inPkt]);
        }

        //刷新编码器缓冲区
        for(int i=0; i<enList.size(); i++){
            if(enList[i] == nullptr){
                continue;
            }

            pktList[outPkt] = av_packet_alloc();
            avcodec_send_frame(enList[i], NULL);
            while (avcodec_receive_packet(enList[i], pktList[outPkt]) >= 0) {
                r = av_interleaved_write_frame(outFmtCtx, pktList[outPkt]);
                if (r < 0) {
                    logError("write frame fail");
                }
            }
            av_packet_unref(pktList[outPkt]);
        }

        //写入文件尾
        r = av_write_trailer(outFmtCtx);
        if(r<0){
            throw QString("write trailer fial");
        }
    } catch(QString e){
        logError(e + " transform code fail");
        return false;
    }

    return true;
}

/*
 * 处理数据帧，进行拆分，拷贝
*/
QList<AVFrame *> FFmpeg::changeFrame(AVFrame *swrFrm)
{
    QList<AVFrame *> outFrmList;
    AVFrame *outFrm = nullptr;
    int pos = 0;
    int sampleNumber = 0;
    int length = 0;
    int frmSample = 0;

    switch (suffix) {
    case MP3:
        frmSample = 1152;
        break;
    case FLAC:
        frmSample = 4096;
        break;
    case AAC:
        frmSample = 1024;
        break;
    case ALAC:
        frmSample = 4096;
        break;
    case PCM16:
        frmSample = 2048;
        break;
    case PCM32:
        frmSample = 2048;
        break;
    case WMA:
        frmSample = 2048;
        break;
    default:
        break;
    }

    //处理不同长度的编码，长样本截断，短样本变化编码长度
    while(pos < swrFrm->nb_samples){
        outFrm = av_frame_alloc();
        outFrm->sample_rate = swrFrm->sample_rate;
        outFrm->format = swrFrm->format;
        outFrm->ch_layout = swrFrm->ch_layout;

        //得到实际帧数并处理短帧
        sampleNumber = pos + frmSample >= swrFrm->nb_samples ? swrFrm->nb_samples - pos : frmSample;
        length = av_get_bytes_per_sample((AVSampleFormat)swrFrm->format);
        outFrm->nb_samples = sampleNumber;
        av_frame_get_buffer(outFrm, 0);

        for(int i=0; i<outFrm->ch_layout.nb_channels; i++){
            memcpy(outFrm->data[i],
                   swrFrm->data[i] + pos * length,
                   length * sampleNumber
                   );
        }
        pos += sampleNumber;
        outFrmList.append(outFrm);
    }
    return outFrmList;
}

bool FFmpeg::getDict(QStringList *keys, QStringList *values, QString url)
{
    try{
        AVFormatContext *inFmt = getInputFormatContext(url);
        AVDictionaryEntry* entry = nullptr;
        while ((entry = av_dict_get(inFmt->metadata, "", entry, AV_DICT_IGNORE_SUFFIX))) {
            QString key(entry->key);
            QString value(entry->value);
            keys->append(key);
            values->append(value);
        }
    }
    catch(QString e){
        logError(e);
        return false;
    }
    return true;
}

bool FFmpeg::writeDict(QStringList key, QStringList value, QString inUrl, QString outUrl)
{
    try{
        AVFormatContext *inFmt = getInputFormatContext(inUrl);
        AVFormatContext *outFmt = getOutFormatContext(outUrl);

        for(int i=0; i<inFmt->nb_streams; i++){
            AVStream *stream = avformat_new_stream(outFmt, nullptr);
            avcodec_parameters_copy(stream->codecpar, inFmt->streams[i]->codecpar);
        }

        for(int i=0; i<key.size(); i++){
            av_dict_set(&outFmt->metadata, key[i].toUtf8(), value[i].toUtf8(), 1);
        }

        r = avformat_write_header(outFmt, nullptr);
        if(r<0){
            throw QString(outUrl + "写入文件头失败");
        }

        AVPacket *pkt = av_packet_alloc();
        while(av_read_frame(inFmt, pkt)){
            av_interleaved_write_frame(outFmt, pkt);
            av_packet_unref(pkt);
        }

        r = av_write_trailer(outFmt);
        if(r<0){
            throw QString(outUrl + "写入文件头失败");
        }
    }
    catch(QString e){
        logError(e);
        return false;
    }
    return true;
}
