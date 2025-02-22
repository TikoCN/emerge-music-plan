#include "ffmpeg.h"

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

int FFmpeg::getAVFormatContext()
{
    AVFormatContext *fmtCtx = nullptr;
    fmtCtxList.append(fmtCtx);
    return fmtCtxList.size() - 1;
}

int FFmpeg::getAVPacket()
{
    AVPacket *pkt = nullptr;
    pkt = av_packet_alloc();
    pktList.append(pkt);
    return pktList.size() - 1;
}

int FFmpeg::getAVCodecContext()
{
    AVCodecContext *codec = nullptr;
    codeCtxList.append(codec);
    return codeCtxList.size() - 1;
}

int FFmpeg::getSwrContext()
{
    SwrContext *swr = nullptr;
    swrList.append(swr);
    return swrList.size() - 1;
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
    qDebug() << "AVCodec : "<< suffix << " error : " << r << " = " << text;
}

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

bool FFmpeg::transformCodec(QString url, Suffix aim)
{
    suffix = aim;
    QString inUrl = url;
    QString outUrl = getOutUrl(url);

    int inFmtCtx = getAVFormatContext();//输入上下文
    int outFmtCtx = getAVFormatContext();//输出上下文
    QVector<SwrContext *>reSwrList;//重采样列表
    QVector<AVCodecContext *>deList;//解码器列表
    QVector<AVCodecContext *>enList;//编码器列表
    QVector<int>indexList;//流转换列表
    int inFrm = getAVFrame();
    int swrFrm = getAVFrame();
    int inPkt = getAVPacket();
    int outPkt = getAVPacket();

    //打开输出文件
    r = avformat_open_input(&fmtCtxList[inFmtCtx], inUrl.toUtf8(), nullptr, nullptr);
    if(r<0){
        logError("open input file fail");
        return false;
    }

    //打开输出文件
    r = avformat_alloc_output_context2(&fmtCtxList[outFmtCtx], nullptr, nullptr, outUrl.toUtf8());
    if(r<0){
        logError("out file open fail");
        return false;
    }

    //查找流信息
    r = avformat_find_stream_info(fmtCtxList[inFmtCtx], nullptr);
    if(r<0){
        logError("no stream find");
        return false;
    }
    //分配字长
    deList.fill(nullptr, fmtCtxList[inFmtCtx]->nb_streams);
    enList.fill(nullptr, fmtCtxList[inFmtCtx]->nb_streams);
    reSwrList.fill(nullptr, fmtCtxList[inFmtCtx]->nb_streams);
    indexList.fill(-1, fmtCtxList[inFmtCtx]->nb_streams);

    //遍历流生成编解码器
    int n = 0;
    for(int i=0; i<fmtCtxList[inFmtCtx]->nb_streams; i++){
        AVStream *inStream = fmtCtxList[inFmtCtx]->streams[i];

        //复制附加封面流
        if(suffix != AAC &&
            suffix != ALAC &&
            inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            inStream->disposition & AV_DISPOSITION_ATTACHED_PIC){
            AVStream * outStream = avformat_new_stream(fmtCtxList[outFmtCtx], nullptr);
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
        AVStream * outStream = avformat_new_stream(fmtCtxList[outFmtCtx], nullptr);
        if(outStream == nullptr){
            logError("get new stream fail");
            return false;
        }
        avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);           //复制流

        //生成解码器
        const AVCodec *decodec = avcodec_find_decoder(inStream->codecpar->codec_id);
        int decodeCtx = getAVCodecContext();                             //申请解码器指针
        codeCtxList[decodeCtx] = avcodec_alloc_context3(decodec);
        avcodec_parameters_to_context(codeCtxList[decodeCtx], inStream->codecpar);
        r = avcodec_open2(codeCtxList[decodeCtx], decodec, nullptr);
        if(r<0){
            logError("open decodec fail");
            return false;
        }
        deList[i] = codeCtxList[decodeCtx];                                                    //添加的解码器列表

        //建立编码器
        int encodeCtx = getAVCodecContext();                             //申请编码器指针
        const AVCodec *encodec = setEncodeParmeters(&codeCtxList[encodeCtx], inStream->codecpar);  //设置编码器流参数
        r = avcodec_open2(codeCtxList[encodeCtx], encodec, nullptr);
        if(r<0){
            logError("open encodec fail");
            return false;
        }
        avcodec_parameters_from_context(outStream->codecpar, codeCtxList[encodeCtx]);
        enList[i] = codeCtxList[encodeCtx];                                                       //添加到编码器列表

        //申请重采样
        int swr = getSwrContext();
        swr_alloc_set_opts2(&swrList[swr],
                            &codeCtxList[encodeCtx]->ch_layout,
                            codeCtxList[encodeCtx]->sample_fmt,
                            codeCtxList[encodeCtx]->sample_rate,
                            &codeCtxList[decodeCtx]->ch_layout,
                            codeCtxList[decodeCtx]->sample_fmt,
                            codeCtxList[decodeCtx]->sample_rate,
                            0, nullptr);
        reSwrList[i] = swrList[swr];
    }

    //拷贝元数据
    AVDictionaryEntry* tag = nullptr;
    while ((tag = av_dict_get(fmtCtxList[inFmtCtx]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        av_dict_set(&fmtCtxList[outFmtCtx]->metadata, tag->key, tag->value, 0);
    }

    if (!(fmtCtxList[outFmtCtx]->oformat->flags & AVFMT_NOFILE)) {
        r = avio_open(&fmtCtxList[outFmtCtx]->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
        if (r < 0) {
            logError("Could not open output file");
            return false;
        }
    }

    r = avformat_write_header(fmtCtxList[outFmtCtx], nullptr);
    if(r<0){
        logError("write header fail");
        return false;
    }
    int64_t pts = 0;

    while(av_read_frame(fmtCtxList[inFmtCtx], pktList[inPkt]) >= 0){
        int index = pktList[inPkt]->stream_index;
        //判断是否保留，不是则跳过
        if(indexList[index] == -1){
            av_packet_unref(pktList[inPkt]);//释放输入
            continue;
        }

        //并非音频流，直接写入
        if(fmtCtxList[inFmtCtx]->streams[index]->codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
            r = av_interleaved_write_frame(fmtCtxList[outFmtCtx], pktList[inPkt]);
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
                    r = av_interleaved_write_frame(fmtCtxList[outFmtCtx], pktList[outPkt]);
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
            r = av_interleaved_write_frame(fmtCtxList[outFmtCtx], pktList[outPkt]);
            if (r < 0) {
                logError("write frame fail");
                return false;
            }
        }
        av_packet_unref(pktList[outPkt]);
    }

    //写入文件尾
    r = av_write_trailer(fmtCtxList[outFmtCtx]);
    if(r<0){
        logError("write trailer fial");
        return false;
    }

    return true;
}

/*
 * 设置编码器参数转码
*/
const AVCodec *FFmpeg::setEncodeParmeters(AVCodecContext **encodecCtx, AVCodecParameters *parameter)
{
    const AVCodec *encodec = nullptr;
    switch (suffix) {
    case MP3:
        encodec = avcodec_find_encoder(AV_CODEC_ID_MP3);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_MP3;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_S32P;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    case FLAC:
        encodec = avcodec_find_encoder(AV_CODEC_ID_FLAC);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_FLAC;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_S16;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 0;
        (*encodecCtx)->time_base.den = (*encodecCtx)->sample_rate;
        (*encodecCtx)->time_base.num = 1;
        (*encodecCtx)->bits_per_raw_sample = 16;
        break;
    case AAC:
        encodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_AAC;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_FLTP;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    case ALAC:
        encodec = avcodec_find_encoder(AV_CODEC_ID_ALAC);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_ALAC;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_S16P;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    case PCM16:
        encodec = avcodec_find_encoder(AV_CODEC_ID_PCM_S16LE);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_PCM_S16LE;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_S16;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    case PCM32:
        encodec = avcodec_find_encoder(AV_CODEC_ID_PCM_S32LE);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_PCM_S32LE;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_S32;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    case WMA:
        encodec = avcodec_find_encoder(AV_CODEC_ID_WMAV2);
        *encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(*encodecCtx, parameter);

        (*encodecCtx)->codec_id = AV_CODEC_ID_WMAV2;
        (*encodecCtx)->codec_type = AVMEDIA_TYPE_AUDIO;
        (*encodecCtx)->sample_fmt = AV_SAMPLE_FMT_FLTP;
        (*encodecCtx)->sample_rate = 44100;
        (*encodecCtx)->bit_rate = 320000;
        break;
    default:
        break;
    }
    return encodec;
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
