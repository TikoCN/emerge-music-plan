#include "ffmpeg.h"

FFmpeg::~FFmpeg()
{
    //关闭输入输出上下文
    while(fmtCtxList.size() > 0){
        avformat_free_context(fmtCtxList.last());

        //如果打开，关闭输出文件
        if (!(fmtCtxList.last()->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&fmtCtxList.last()->pb);
        }

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

AVFormatContext *FFmpeg::getAVFormatContext()
{
    AVFormatContext *fmtCtx = nullptr;
    fmtCtxList.append(fmtCtx);
    return fmtCtx;
}

AVPacket *FFmpeg::getAVPacket()
{
    AVPacket *pkt = nullptr;
    pktList.append(pkt);
    pkt = av_packet_alloc();
    return pkt;
}

AVCodecContext *FFmpeg::getAVCodecContext()
{
    AVCodecContext *codec = nullptr;
    codeCtxList.append(codec);
    return codec;
}

SwrContext *FFmpeg::getSwrContext()
{
    SwrContext *swr = nullptr;
    swrList.append(swr);
    return swr;
}

AVFrame *FFmpeg::getAVFrame()
{
    AVFrame *frm = nullptr;
    frmList.append(frm);
    frm = av_frame_alloc();
    return frm;
}

void FFmpeg::logError(QString text){
    char error[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(r, error, AV_ERROR_MAX_STRING_SIZE);
}

bool FFmpeg::toFlac(QString url){
    // QString inUrl = url;
    // QString suffix = url.split(".").last();
    // QString outUrl = url.replace("."+suffix, ".flac");

    // r = avformat_open_input(&inFmtCtx, inUrl.toUtf8(), nullptr, nullptr);
    // if(r<0){
    //     logError("open input file fail");
    //     return false;
    // }

    // r = avformat_find_stream_info(inFmtCtx, nullptr);
    // if(r<0){
    //     logError("no stream find");
    //     return false;
    // }

    // r = av_find_best_stream(inFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    // if(r<0){
    //     logError("no audio stream find");
    //     return false;
    // }
    // inAudioStream = inFmtCtx->streams[r];

    // //生成解码器
    // const AVCodec *decodec = avcodec_find_decoder(inAudioStream->codecpar->codec_id);
    // decodeCtx = avcodec_alloc_context3(decodec);
    // avcodec_parameters_to_context(decodeCtx, inAudioStream->codecpar);
    // r = avcodec_open2(decodeCtx, decodec, nullptr);
    // if(r<0){
    //     logError("open decodec fail");
    //     return false;
    // }

    // //打开输出文件
    // r = avformat_alloc_output_context2(&outFmtCtx, nullptr, nullptr, outUrl.toUtf8());
    // if(r<0){
    //     logError("outUrlile open fail");
    //     return false;
    // }
    // //时长flac流
    // outAudioStream = avformat_new_stream(outFmtCtx, nullptr);
    // if(outAudioStream == NULL){
    //     logError("get new stream fail");
    //     return false;
    // }

    // const AVCodec *encodec = avcodec_find_encoder(AV_CODEC_ID_FLAC);
    // encodeCtx = avcodec_alloc_context3(encodec);
    // avcodec_parameters_to_context(encodeCtx, inAudioStream->codecpar);
    // encodeCtx->codec_id = AV_CODEC_ID_FLAC;
    // encodeCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    // encodeCtx->ch_layout = decodeCtx->ch_layout;
    // encodeCtx->sample_fmt = AV_SAMPLE_FMT_S16;
    // encodeCtx->sample_rate = 44100;
    // encodeCtx->bit_rate = 0;
    // encodeCtx->time_base.den = encodeCtx->sample_rate;
    // encodeCtx->time_base.num = 1;
    // encodeCtx->bits_per_raw_sample = 16;
    // r = avcodec_open2(encodeCtx, encodec, nullptr);
    // if(r<0){
    //     logError("open encodec fail");
    //     return false;
    // }
    // avcodec_parameters_from_context(outAudioStream->codecpar, encodeCtx);

    // //拷贝元数据
    // AVDictionaryEntry* tag = nullptr;
    // while ((tag = av_dict_get(inFmtCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
    //     av_dict_set(&outFmtCtx->metadata, tag->key, tag->value, 0);
    // }

    // if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
    //     r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
    //     if (r < 0) {
    //         logError("Could not open output file");
    //         return false;
    //     }
    // }

    // r = avformat_write_header(outFmtCtx, nullptr);
    // if(r<0){
    //     logError("write header fail");
    //     return false;
    // }

    // inPak = av_packet_alloc();
    // inFrame = av_frame_alloc();
    // swr_alloc_set_opts2(&swr,
    //                     &encodeCtx->ch_layout, encodeCtx->sample_fmt, encodeCtx->sample_rate,
    //                     &decodeCtx->ch_layout, decodeCtx->sample_fmt, decodeCtx->sample_rate,
    //                     0, nullptr);

    // int64_t pts = 0;

    // while(av_read_frame(inFmtCtx, inPak) >= 0){

        //     if(inPak->stream_index != inAudioStream->index){

    //     }
    //     r = avcodec_send_packet(decodeCtx, inPak);
    //     if(r<0){
    //         logError("send packet fail");
    //     }

    //     while(avcodec_receive_frame(decodeCtx, inFrame) >= 0){
    //         outFrame = av_frame_alloc();
    //         outFrame->sample_rate = encodeCtx->sample_rate;
    //         outFrame->format = encodeCtx->sample_fmt;
    //         outFrame->ch_layout = encodeCtx->ch_layout;

    //         r = swr_convert_frame(swr, outFrame, inFrame);
    //         if(r<0){
    //             logError("swr fail");
    //         }
    //         encodeCtx->frame_size = outFrame->nb_samples;

    //         r = avcodec_send_frame(encodeCtx, outFrame);
    //         if(r<0){
    //             logError("send frame fail");
    //         }

    //         outPak = av_packet_alloc();
    //         while(avcodec_receive_packet(encodeCtx, outPak) >= 0){
    //             r = av_interleaved_write_frame(outFmtCtx, outPak);
    //             if(r<0){
    //                 logError("write frame fail");
    //             }
    //         }
    //         av_packet_unref(outPak);
    //         av_frame_unref(outFrame);
    //     }
    // }

    // //刷新编码器缓冲区
    // avcodec_send_frame(encodeCtx, nullptr);
    // while (avcodec_receive_packet(encodeCtx, outPak) >= 0) {
    //     r = av_interleaved_write_frame(outFmtCtx, outPak);
    //     if (r < 0) {
    //         logError("write frame fail");
    //         return false;
    //     }
    //     av_packet_unref(outPak);
    // }
    // av_packet_free(&outPak);

    // r = av_write_trailer(outFmtCtx);
    // if(r<0){
    //     logError("write trailer fial");
    //     return false;
    // }

    // closeAll();
    return true;
}

bool FFmpeg::toMp3(QString url){
    QString inUrl = url;
    QString suffix = url.split(".").last();
    const QString aim = "mp3";
    QString outUrl;
    if(suffix == aim){
        outUrl = url.replace("."+suffix, "-New."+aim);
    }
    else{
        outUrl = url.replace("."+suffix, "."+aim);
    }

    AVFormatContext *inFmtCtx = getAVFormatContext();//输入上下文
    AVFormatContext *outFmtCtx = getAVFormatContext();//输出上下文
    QVector<SwrContext *>swrList;//重采样列表
    QVector<AVCodecContext *>deList;//解码器列表
    QVector<AVCodecContext *>enList;//编码器列表
    QVector<int>indexList;//流转换列表
    AVFrame *inFrm = getAVFrame();
    AVFrame *swrFrm = getAVFrame();
    AVPacket *inPkt = getAVPacket();
    AVPacket *outPkt = getAVPacket();

    //打开输出文件
    r = avformat_open_input(&inFmtCtx, inUrl.toUtf8(), nullptr, nullptr);
    if(r<0){
        logError("open input file fail");
        return false;
    }

    //打开输出文件
    r = avformat_alloc_output_context2(&outFmtCtx, nullptr, nullptr, outUrl.toUtf8());
    if(r<0){
        logError("outUrlile open fail");
        return false;
    }

    //查找流信息
    r = avformat_find_stream_info(inFmtCtx, nullptr);
    if(r<0){
        logError("no stream find");
        return false;
    }
    //分配字长
    deList.fill(nullptr, inFmtCtx->nb_streams);
    enList.fill(nullptr, inFmtCtx->nb_streams);
    swrList.fill(nullptr, inFmtCtx->nb_streams);
    indexList.fill(-1, inFmtCtx->nb_streams);

    //遍历流生成编解码器
    int n = 0;
    for(int i=0; i<inFmtCtx->nb_streams; i++){
        AVStream *inStream = inFmtCtx->streams[i];

        //复制附加封面流
        if(inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && inStream->disposition & AV_DISPOSITION_ATTACHED_PIC){
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
            logError("get new stream fail");
            return false;
        }
        avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);           //复制流

        //生成解码器
        const AVCodec *decodec = avcodec_find_decoder(inStream->codecpar->codec_id);
        AVCodecContext *decodeCtx = getAVCodecContext();                             //申请解码器指针
        deList[i] = decodeCtx;                                                    //添加的解码器列表

        decodeCtx = avcodec_alloc_context3(decodec);
        avcodec_parameters_to_context(decodeCtx, inStream->codecpar);
        r = avcodec_open2(decodeCtx, decodec, nullptr);
        if(r<0){
            logError("open decodec fail");
            return false;
        }

        //建立编码器
        AVCodecContext *encodeCtx = getAVCodecContext();                             //申请编码器指针
        enList[i] = encodeCtx;                                                       //添加到编码器列表
        const AVCodec *encodec = setEncodeParmeters(encodeCtx, inStream->codecpar);                                               //设置编码器流参数

        r = avcodec_open2(encodeCtx, encodec, nullptr);
        if(r<0){
            logError("open encodec fail");
            return false;
        }
        avcodec_parameters_from_context(outStream->codecpar, encodeCtx);

        //申请重采样
        SwrContext *swr = getSwrContext();
        swrList[i] = swr;
        swr_alloc_set_opts2(&swr,
                            &encodeCtx->ch_layout, encodeCtx->sample_fmt, encodeCtx->sample_rate,
                            &decodeCtx->ch_layout, decodeCtx->sample_fmt, decodeCtx->sample_rate,
                            0, nullptr);
    }

    //拷贝元数据
    AVDictionaryEntry* tag = nullptr;
    while ((tag = av_dict_get(inFmtCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        av_dict_set(&outFmtCtx->metadata, tag->key, tag->value, 0);
    }

    if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
        if (r < 0) {
            logError("Could not open output file");
            return false;
        }
    }

    r = avformat_write_header(outFmtCtx, nullptr);
    if(r<0){
        logError("write header fail");
        return false;
    }
    int64_t pts = 0;

    while(av_read_frame(inFmtCtx, inPkt) >= 0){
        int index = inPkt->stream_index;
        //判断是否保留，不是则跳过
        if(indexList[index] != -1){
            av_packet_unref(inPkt);
            continue;
        }

        //并非音频流，直接写入
        if(inFmtCtx->streams[index]->codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
            r = av_interleaved_write_frame(outFmtCtx, outPkt);
            if(r<0){
                logError("write frame fail");
                return false;
            }
            continue;
        }

        //音频流进行解码，重采样，编码
        AVCodecContext *decodeCtx = deList[index];
        SwrContext *swr = swrList[index];
        AVCodecContext *encodeCtx = enList[index];
        //解码数据
        r = avcodec_send_packet(decodeCtx, inPkt);
        if(r<0){
            logError("send packet fail");
            return false;
        }

        //获得解码之后的数据
        while(avcodec_receive_frame(decodeCtx, inFrm) >= 0){
            swrFrm->sample_rate = encodeCtx->sample_rate;
            swrFrm->format = encodeCtx->sample_fmt;
            swrFrm->ch_layout = encodeCtx->ch_layout;

            //重采样
            r = swr_convert_frame(swr, swrFrm, inFrm);
            if(r<0){
                logError("swr fail");
                return false;
            }

            QList<AVFrame *>outFrmList = changeFrame(swrFrm);
            for(int i=0; i<outFrmList.size(); i++){
                //设置帧样本大小
                encodeCtx->frame_size = outFrmList[i]->nb_samples;

                r = avcodec_send_frame(encodeCtx, outFrmList[i]);
                if(r<0){
                    logError("send frame fail");
                    av_frame_unref(outFrmList[i]);
                    av_frame_free(&outFrmList[i]);
                    continue;
                }

                outPkt = av_packet_alloc();
                while(avcodec_receive_packet(encodeCtx, outPkt) >= 0){
                    r = av_interleaved_write_frame(outFmtCtx, outPkt);
                    if(r<0){
                        logError("write frame fail");
                    }
                }
                av_packet_unref(outPkt);
                av_frame_unref(outFrmList[i]);
                av_frame_free(&outFrmList[i]);
            }
            av_frame_unref(swrFrm);
        }
    }


    //刷新编码器缓冲区
    for(int i=0; i<enList.size(); i++){
        if(enList[i] == nullptr){
            continue;
        }

        outPkt = av_packet_alloc();
        avcodec_send_frame(enList[i], NULL);
        while (avcodec_receive_packet(enList[i], outPkt) >= 0) {
            r = av_interleaved_write_frame(outFmtCtx, outPkt);
            if (r < 0) {
                logError("write frame fail");
                return false;
            }
        }
        av_packet_unref(outPkt);
    }

    //写入文件尾
    r = av_write_trailer(outFmtCtx);
    if(r<0){
        logError("write trailer fial");
        return false;
    }

    return true;
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
    case M4A:
        aim = "m4a";
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

bool FFmpeg::transformCodec(QString url, Suffix)
{
    return true;
}

const AVCodec *FFmpeg::setEncodeParmeters(AVCodecContext *encodecCtx, AVCodecParameters *parameter)
{
    const AVCodec *encodec = nullptr;
    switch (suffix) {
    case MP3:
        encodec = avcodec_find_encoder(AV_CODEC_ID_MP3);
        encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(encodecCtx, parameter);

        encodecCtx->codec_id = AV_CODEC_ID_MP3;
        encodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        encodecCtx->sample_fmt = AV_SAMPLE_FMT_S32P;
        encodecCtx->sample_rate = 44100;
        encodecCtx->bit_rate = 320000;
        break;
    case FLAC:
        encodec = avcodec_find_encoder(AV_CODEC_ID_FLAC);
        encodecCtx = avcodec_alloc_context3(encodec);
        avcodec_parameters_to_context(encodecCtx, parameter);

        encodecCtx->codec_id = AV_CODEC_ID_FLAC;
        encodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
        encodecCtx->sample_fmt = AV_SAMPLE_FMT_S16;
        encodecCtx->sample_rate = 44100;
        encodecCtx->bit_rate = 0;
        encodecCtx->time_base.den = encodecCtx->sample_rate;
        encodecCtx->time_base.num = 1;
        encodecCtx->bits_per_raw_sample = 16;
        break;
    case AAC:

        break;
    case M4A:

        break;
    default:
        break;
    }
    return encodec;
}

QList<AVFrame *> FFmpeg::changeFrame(AVFrame *swrFrm)
{
    QList<AVFrame *> frmList;
    AVFrame *outFrm = nullptr;
    switch (suffix) {
    case MP3:
    {
        //处理不同长度的编码，长样本截断，短样本变化编码长度
        int pos = 0;
        while(pos < swrFrm->nb_samples){
            outFrm = av_frame_alloc();
            av_frame_copy_props(outFrm, swrFrm);
            frmList.append(outFrm);

            //得到实际帧数并处理短帧
            int sampleNumber = pos + 1152 >= swrFrm->nb_samples ? swrFrm->nb_samples - pos : 1152;
            int length = av_get_bytes_per_sample((AVSampleFormat)swrFrm->format);
            outFrm->nb_samples = sampleNumber;
            av_frame_get_buffer(outFrm, 0);

            for(int i=0; i<outFrm->ch_layout.nb_channels; i++){
                memcpy(outFrm->data[i],
                       swrFrm->data[i] + pos * length,
                       length * sampleNumber
                       );
            }
            pos += sampleNumber;
        }
    }
        break;
    case FLAC:
        outFrm = av_frame_alloc();
        av_frame_copy_props(outFrm, swrFrm);
        av_frame_copy(outFrm, swrFrm);
        frmList.append(outFrm);
        break;
    case AAC:

        break;
    case M4A:

        break;
    default:
        break;
    }
    return frmList;
}
