#include "ffmpeg.h"
#include <QImage>
#include <QDir>
#include "baseclass/dataexception.h"

QString FFmpeg::suffixToString(const Suffix s)
{
    switch (s) {
    case MP3: return "mp3";
    case FLAC: return "flac";
    case AAC: return "aac";
    case ALAC: return "alac";
    case WMA: return "wma";
    default: return "none";
    }
}

/*
 * 得到输出上下文
 */
AVFormatContext *FFmpeg::getOutFormatContext(const QString& url)
{
    AVFormatContext *fmtCtx = nullptr;
    //打开输出文件
    m_r = avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, url.toUtf8());
    if(m_r<0){
        throw DataException(url + tr("output file open fail"));
    }

    return fmtCtx;
}

/*
 * 得到输入上下文
 */
AVFormatContext *FFmpeg::getInputFormatContext(const QString& url)
{
    AVFormatContext *fmtCtx = nullptr;
    //打开输出文件
    m_r = avformat_open_input(&fmtCtx, url.toUtf8(), nullptr, nullptr);
    if(m_r<0){
        throw DataException(url + tr("open input file fail"));
    }

    //查找流信息
    m_r = avformat_find_stream_info(fmtCtx, nullptr);
    if(m_r<0){
        throw DataException(url + tr("no stream find"));
    }

    return fmtCtx;
}

/*
 * 得到解码器
 */
AVCodecContext *FFmpeg::getDecodecContext(const AVCodecID id, const AVCodecParameters *para)
{
    //寻找编解码器
    const AVCodec *codec = avcodec_find_encoder(id);
    if(!codec){
        throw DataException(tr("no find codec"));
    }
    const QString name = QString::fromUtf8(codec->name);

    //生成编解码上下文
    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx){
        throw DataException(name + ("no find codec"));
    }

    m_r = avcodec_parameters_to_context(codecCtx, para);
    if(m_r<0){
        throw DataException(name + tr("copy parameter to context fail"));
    }

    m_r = avcodec_open2(codecCtx, codec, nullptr);
    if(m_r < 0){
        throw DataException("decodecContext and encodec open fail");
    }

    return codecCtx;
}

/*
 * 得到编码器
 */
AVCodecContext *FFmpeg::getEncodecContext(AVCodecID id, const AVCodecParameters *para, bool defaultCodeFlag)
{
    //选择默认编码器类型
    if(defaultCodeFlag){
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
    if (!codec) {
        throw DataException(suffixToString(suffix) + tr("no find codec"));
    }

    //生成编解码上下文
    AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        throw DataException(suffixToString(suffix) + tr("no find codec"));
    }

    m_r = avcodec_parameters_to_context(codecCtx, para);
    if (m_r < 0) {
        throw DataException(suffixToString(suffix) + tr("copy parameter to context fail"));
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

    m_r = avcodec_open2(codecCtx, codec, nullptr);
    if(m_r < 0){
        throw DataException(QString::fromUtf8(codec->name) + " encodecContext and codec open fail");
    }

    return codecCtx;
}

AVPacket *FFmpeg::getAVPacket()
{
    AVPacket *pkt = nullptr;
    pkt = av_packet_alloc();
    if (pkt == nullptr) {
        throw DataException("分配AVPacket数据包失败");
    }
    return pkt;
}

SwrContext *FFmpeg::getSwrContext(const AVCodecContext *out, const AVCodecContext *in)
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
    if (swr == nullptr) {
        throw DataException("分配SwrContext重采样失败");
    }
    return swr;
}

AVFrame *FFmpeg::getAVFrame()
{
    AVFrame *frm = nullptr;
    frm = av_frame_alloc();
    if (frm == nullptr) {
        throw DataException("分配AVFrame数据帧失败");
    }
    return frm;
}

/*
 * 打印错误日志
 */
void FFmpeg::logError(const QString &text) const{
    char error[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(m_r, error, AV_ERROR_MAX_STRING_SIZE);
    qDebug() << "ffmpeg error number : " + QString::number(m_r) + " = error, working " + text;
}

/*
 * 从音乐文件中提取独立封面
 */
QImage FFmpeg::getInlayCover(const QString& url)
{
    //从文件中读取封面
    AVFormatContext *fmtCtx = nullptr;
    QImage img;
    try{
        fmtCtx = getInputFormatContext(url);

        for (unsigned int i = 0; i < fmtCtx->nb_streams; i++){
            const AVStream *stream = fmtCtx->streams[i];
            if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && stream->disposition & AV_DISPOSITION_ATTACHED_PIC){
                const AVPacket pkt = stream->attached_pic;
                //使用QImage读取完整图片数据
                img = QImage::fromData((uchar*)pkt.data, pkt.size);
            }
        }
    }
    catch(DataException& e){
        logError(e.errorMessage());
    }

    if (fmtCtx != nullptr) avformat_close_input(&fmtCtx);
    return img;
}

/*
 * 将音乐文件内嵌的音乐文件中
 */
void FFmpeg::setInlayCover(const QString& musicUrl, const QString& coverUrl)
{
    AVFormatContext *inFmtCtx = nullptr;
    AVFormatContext *outFmtCtx = nullptr;
    AVPacket *inPkt = nullptr;
    try{
        AVStream *outStream = nullptr;
        const AVStream *inStream = nullptr;
        QString outUrl = musicUrl;
        //设置内嵌封面之后的路径
        outUrl.replace("." + outUrl.split(".").last(), "-New." + outUrl.split(".").last());

        //探测音乐文件，和封面文件是否存在
        if(!QFile::exists(musicUrl)){
            throw DataException("no find music file");
        }
        if(!QFile::exists(coverUrl)){
            throw DataException("no find cover file");
        }

        //读取封面中的数据
        QFile cover(coverUrl);
        if(!cover.open(QIODevice::ReadOnly)){
            cover.close();
            throw DataException("open cover file fail");
        }
        QByteArray data = cover.readAll();
        cover.close();

        // 分配 输入，输出上下文
        inFmtCtx = getInputFormatContext(musicUrl);
        outFmtCtx = getOutFormatContext(outUrl);

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

        m_r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
        if(m_r<0){
            throw DataException(outUrl + " pb open fail");
        }

        //写入文件头
        m_r = avformat_write_header(outFmtCtx, nullptr);
        if(m_r<0){
            throw DataException(outUrl + " write header fail");
        }

        inPkt = getAVPacket();
        while(av_read_frame(inFmtCtx, inPkt) >= 0){
            if(inPkt->stream_index == aimStream){
                //放弃旧数据
                continue;
            }
            av_interleaved_write_frame(outFmtCtx, inPkt);
        }

        //写入封面包
        av_packet_unref(inPkt);
        av_new_packet(inPkt, static_cast<int>(data.size()));

        //拷贝数据包
        memcpy(inPkt->data, data.data(), data.size());
        inPkt->stream_index = aimStream;
        inPkt->pts = 0;
        inPkt->duration = inFmtCtx->duration;
        av_interleaved_write_frame(outFmtCtx, inPkt);

        //写入文件尾
        m_r = av_write_trailer(outFmtCtx);
        if(m_r<0){
            throw DataException(outUrl + " write trailer fail");
        }
    }
    catch(const DataException &e){
        logError(e.errorMessage() + tr("inlay cover write fail"));
    }
    if (inPkt != nullptr) {
        av_packet_unref(inPkt);
        av_packet_free(&inPkt);
    }

    //关闭输出上下文
    if (outFmtCtx != nullptr) {
        //如果打开，关闭输出文件
        if (outFmtCtx->oformat != nullptr && !(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&outFmtCtx->pb);
        }
        avformat_free_context(outFmtCtx);
    }
    if(inFmtCtx != nullptr) avformat_close_input(&inFmtCtx);
}

/*
 * 得到输出文件路径
 */
QString FFmpeg::getOutUrl(QString inUrl) const
{
    const QString inSuffix = inUrl.split(".").last();
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
 bool FFmpeg::transformCodec(const QString& url, const Suffix aim)
{
    suffix = aim;
    const QString& inUrl = url;
    const QString outUrl = getOutUrl(url);

    AVFormatContext *inFmtCtx = nullptr;//输入上下文
    AVFormatContext *outFmtCtx = nullptr;//输出上下文
    QVector<SwrContext *>reSwrList;//重采样列表
    QVector<AVCodecContext *>deList;//解码器列表
    QVector<AVCodecContext *>enList;//编码器列表
    AVPacket *inPkt = nullptr, *outPkt = nullptr;
    AVFrame *inFrm = nullptr, *swrFrm = nullptr;
    bool work = true;
    try{
        inFmtCtx = getInputFormatContext(inUrl);//输入上下文
        outFmtCtx = getOutFormatContext(outUrl);//输出上下文
        QVector<int>indexList;//流转换列表

        //分配字长
        deList.fill(nullptr, inFmtCtx->nb_streams);
        enList.fill(nullptr, inFmtCtx->nb_streams);
        reSwrList.fill(nullptr, inFmtCtx->nb_streams);
        indexList.fill(-1, inFmtCtx->nb_streams);

        //遍历流生成编解码器
        int n = 0;
        for(int i=0; i<inFmtCtx->nb_streams; i++){
            const AVStream *inStream = inFmtCtx->streams[i];

            //复制附加封面流
            if(suffix != AAC &&
                suffix != ALAC &&
                inStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
                inStream->disposition & AV_DISPOSITION_ATTACHED_PIC){
                const AVStream * outStream = avformat_new_stream(outFmtCtx, nullptr);
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
            const AVStream * outStream = avformat_new_stream(outFmtCtx, nullptr);
            if(outStream == nullptr){
                throw DataException("get new stream fail");
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
            m_r = avio_open(&outFmtCtx->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
            if (m_r < 0) {
                throw DataException("Could not open output file");
            }
        }

        m_r = avformat_write_header(outFmtCtx, nullptr);
        if(m_r<0){
            throw DataException("write header fail");
        }

        //读取数据设置循环解码
        inPkt = getAVPacket();
        outPkt = getAVPacket();
        inFrm = getAVFrame();
        swrFrm = getAVFrame();
        while(av_read_frame(inFmtCtx, inPkt) >= 0){
            const int index = inPkt->stream_index;
            //判断是否保留，不是则跳过
            if(indexList[index] == -1){
                av_packet_unref(inPkt);//释放输入
                continue;
            }

            //并非音频流，直接写入
            if(inFmtCtx->streams[index]->codecpar->codec_type != AVMEDIA_TYPE_AUDIO){
                m_r = av_interleaved_write_frame(outFmtCtx, inPkt);
                if(m_r<0){
                    logError("write frame fail");
                }
                av_packet_unref(inPkt);//释放输入
                continue;
            }

            //音频流进行解码，重采样，编码
            AVCodecContext *decodeCtx = deList[index];
            SwrContext *swr = reSwrList[index];
            AVCodecContext *encodeCtx = enList[index];
            //解码数据
            m_r = avcodec_send_packet(decodeCtx, inPkt);
            if(m_r<0){
                logError("send packet fail");
            }

            //获得解码之后的数据
            while(avcodec_receive_frame(decodeCtx, inFrm) >= 0){
                swrFrm->sample_rate = encodeCtx->sample_rate;
                swrFrm->format = encodeCtx->sample_fmt;
                swrFrm->ch_layout = encodeCtx->ch_layout;

                //重采样
                m_r = swr_convert_frame(swr, swrFrm, inFrm);
                if(m_r<0){
                    logError("swr fail");
                }

                QList<AVFrame *>outFrmList = changeFrame(swrFrm);
                for(AVFrame *& i : outFrmList){
                    //设置帧样本大小
                    //encodeCtx->frame_size = outFrmList[i]->nb_samples;

                    m_r = avcodec_send_frame(encodeCtx, i);
                    if(m_r<0){
                        logError("send frame fail");
                        av_frame_unref(i);
                        av_frame_free(&i);
                        continue;
                    }

                    while(avcodec_receive_packet(encodeCtx, outPkt) >= 0){
                        m_r = av_interleaved_write_frame(outFmtCtx, outPkt);
                        if(m_r<0){
                            logError("write frame fail");
                        }
                        av_packet_unref(outPkt);
                    }
                    av_frame_unref(i);
                    av_frame_free(&i);
                    av_frame_unref(swrFrm);
                }
                av_frame_unref(inFrm);
            }
            av_packet_unref(inPkt);
        }

        //刷新编码器缓冲区
        for(const auto & i : enList){
            if(i == nullptr){
                continue;
            }

            avcodec_send_frame(i, nullptr);
            while (avcodec_receive_packet(i, outPkt) >= 0) {
                m_r = av_interleaved_write_frame(outFmtCtx, outPkt);
                if (m_r < 0) {
                    logError("write frame fail");
                }
            }
            av_packet_unref(outPkt);
        }

        //写入文件尾
        m_r = av_write_trailer(outFmtCtx);
        if(m_r<0){
            throw DataException("write trailer fail");
        }
    } catch(const DataException &e){
        logError(e.errorMessage() + tr("transform code fail"));
        work = false;
    }

    if (inPkt != nullptr) {
        av_packet_unref(inPkt);
        av_packet_free(&inPkt);
    }
    if (outPkt != nullptr) {
        av_packet_unref(outPkt);
        av_packet_free(&outPkt);
    }
    if (inFrm != nullptr) {
        av_frame_unref(inFrm);
        av_frame_free(&inFrm);
    }
    if (swrFrm != nullptr) {
        av_frame_unref(swrFrm);
        av_frame_free(&swrFrm);
    }

    //关闭输出上下文
    if (outFmtCtx != nullptr) {
        //如果打开，关闭输出文件
        if (outFmtCtx->oformat != nullptr && !(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&outFmtCtx->pb);
        }
        avformat_free_context(outFmtCtx);
    }
    if(inFmtCtx != nullptr) avformat_close_input(&inFmtCtx);

    while (!reSwrList.empty()) {
        SwrContext *swr = reSwrList.takeLast();
        swr_free(&swr);
    }
    while (!deList.empty()) {
        AVCodecContext *ctx = deList.takeLast();
        avcodec_free_context(&ctx);
    }
    for (AVCodecContext * ctx : deList) {
        avcodec_free_context(&ctx);
    }
    while (!enList.empty()) {
        AVCodecContext *ctx = enList.takeLast();
        avcodec_free_context(&ctx);
    }

    return work;
}

/*
 * 处理数据帧，进行拆分，拷贝
*/
QList<AVFrame *> FFmpeg::changeFrame(AVFrame *swrFrm) const
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
        case AAC:
            frmSample = 1024;
            break;
        case FLAC:
        case ALAC:
            frmSample = 4096;
            break;
        case PCM16:
        case PCM32:
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
        length = av_get_bytes_per_sample(static_cast<AVSampleFormat>(swrFrm->format));
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

bool FFmpeg::getDict(QStringList *keys, QStringList *values, const QString& url)
{
    AVFormatContext *inFmt = nullptr;
    bool work = true;
    try{
        inFmt = getInputFormatContext(url);
        const AVDictionaryEntry* entry = nullptr;
        while ((entry = av_dict_get(inFmt->metadata, "", entry, AV_DICT_IGNORE_SUFFIX))) {
            QString key(entry->key);
            QString value(entry->value);
            keys->append(key);
            values->append(value);
        }
        // 插入其他数据
        keys->append("duration");
        int64_t duration = inFmt->duration;
        int64_t durationMs = (duration * 1000) / AV_TIME_BASE;
        values->append(QString::number(durationMs));
    }
    catch(const DataException &e){
        logError(e.errorMessage());
        work = false;
    }
    if(inFmt != nullptr) avformat_close_input(&inFmt);
    return work;
}

bool FFmpeg::getDict(MediaData *data, const QString& url)
{
    QStringList keyList;
    QStringList valueList;
    bool r = getDict(&keyList, &valueList, url);

    data->url = url;
    const QString name = url.split("/").last();
    data->dir = url.split("/"+name)[0];

    for (int i = 0; i < keyList.size(); ++i) {
        if (keyList[i].compare("title", Qt::CaseInsensitive) == 0) {
            QString title = valueList[i];
            if (title == "" || title.replace(" ", "") == "") title = tr("未知歌曲");
            data->title = title;
        }
        else if (keyList[i].compare("artist", Qt::CaseInsensitive) == 0) {
            QString artist = valueList[i];
            if (artist == "" || artist.replace(" ", "") == "") artist = tr("未知歌手");
            data->artistList = artist.split(";");
        }
        else if (keyList[i].compare("album", Qt::CaseInsensitive) == 0) {
            QString album = valueList[i];
            if (album == "" || album.replace(" ", "") == "") album = tr("未知专辑");
            data->album = album;
        }
        else if (keyList[i].compare("duration", Qt::CaseInsensitive) == 0) {
            data->duration = valueList[i].toLongLong();
        }
        else if (keyList[i].compare("level", Qt::CaseInsensitive) == 0) {
            data->level = valueList[i].toInt();
        }
        else if (keyList[i].compare("love", Qt::CaseInsensitive) == 0) {
            data->isLove = valueList[i].toInt() == 1;
        }
        else if (keyList[i].compare("playNumber", Qt::CaseInsensitive) == 0) {
            data->playNumber = valueList[i].toInt();
        }
    }
    return r;
}

bool FFmpeg::writeDict(QStringList key, QStringList value, const QString& inUrl, const QString& outUrl)
{
    AVFormatContext *inFmt = nullptr;
    AVFormatContext *outFmt = nullptr;
    AVPacket *pkt = nullptr;
    bool work = true;
    try{
        if (key.size() != value.size()) {
            throw DataException("写入标签数据长度错误");
        }

        inFmt = getInputFormatContext(inUrl);
        outFmt = getOutFormatContext(outUrl);

        for(int i=0; i<inFmt->nb_streams; i++){
            AVStream *stream = avformat_new_stream(outFmt, nullptr);
            if (!stream) {
                throw DataException("无法创建输出流");
            }
            avcodec_parameters_copy(stream->codecpar, inFmt->streams[i]->codecpar);
            stream->time_base = inFmt->streams[i]->time_base;
        }

        // 打开输出文件
        if (!(outFmt->oformat->flags & AVFMT_NOFILE)) {
            m_r = avio_open(&outFmt->pb, outUrl.toUtf8(), AVIO_FLAG_WRITE);
            if (m_r < 0) {
                throw DataException(outUrl + "输出文件打开失败");
            }
        }

        // 复制标签
        av_dict_copy(&outFmt->metadata, inFmt->metadata, 1);

        // 写入新的标签
        for(int i=0; i<key.size(); i++){
            av_dict_set(&outFmt->metadata, key[i].toUtf8(), value[i].toUtf8(), AV_DICT_DONT_OVERWRITE);
        }

        m_r = avformat_write_header(outFmt, nullptr);
        if(m_r<0){
            throw DataException(outUrl + "写入文件头失败");
        }

        pkt = getAVPacket();
        while(av_read_frame(inFmt, pkt) >= 0){
            av_interleaved_write_frame(outFmt, pkt);
            av_packet_unref(pkt);
        }
        av_packet_free(&pkt);

        m_r = av_write_trailer(outFmt);
        if(m_r<0){
            throw DataException(outUrl + "写入文件尾失败");
        }
    }
    catch(const DataException &e){
        logError(e.errorMessage());
        work = false;
    }

    if (pkt != nullptr) av_packet_free(&pkt);
    //关闭输出上下文
    if (outFmt != nullptr) {
        //如果打开，关闭输出文件
        if (outFmt->oformat != nullptr && !(outFmt->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&outFmt->pb);
        }
        avformat_free_context(outFmt);
    }
    if(inFmt != nullptr) avformat_close_input(&inFmt);
    return work;
}
