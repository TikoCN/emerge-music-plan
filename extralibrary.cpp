#include "extralibrary.h"
#include "popupdata.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

#include "fftw3.h"
#include "taglib.h"
#include "fileref.h"
#include "tstring.h"
#include "audioproperties.h"

#include <QPainter>
#include <QPainterPath>

/*
*设置文件参数
*/
bool ExtraLibrary::setMedia(QString url, QString title, QString artist, QString alumb, QString genre, QString year)
{
    PopupData* pop = PopupData::getInstance();
    TagLib::FileRef f(TagLib::String(url.toStdString(), TagLib::String::UTF8).toCWString());

    if(f.isNull()){
        pop->message(url + "打开文件失败");
        return false;
    }

    f.tag()->setTitle(TagLib::String(title.toStdString(), TagLib::String::UTF8));
    f.tag()->setArtist(TagLib::String(artist.toStdString(), TagLib::String::UTF8));
    f.tag()->setAlbum(TagLib::String(alumb.toStdString(), TagLib::String::UTF8));
    f.tag()->setYear(year.toInt());
    f.tag()->setGenre(TagLib::String(genre.toStdString(), TagLib::String::UTF8));

    if(!f.save()){
        pop->message(url + "保存文件失败");
        return false;
    }

    return true;
}

/*
*得到文件参数
*/
void ExtraLibrary::getMedia(Music* core)
{
    PopupData* pop = PopupData::getInstance();
    TagLib::FileRef f(TagLib::String(core->url.toStdString(), TagLib::String::UTF8).toCWString());
    if(f.isNull()){
        pop->message(core->url + "打开文件失败");
        return;
    }
    TagLib::Tag* t = f.tag();

    //读取信息
    core->title = QString::fromStdString(t->title().toCString(1));
    if(core->title == "" || QString(core->title).replace(" ", "") == ""){
        core->title = core->getBaseName();
    }
    core->artist = QString::fromStdString(t->artist().toCString(1));
    if(core->artist == "" || QString(core->artist).replace(" ", "") == ""){
        core->artist = QObject::tr("未知歌手");
    }
    core->alumb = QString::fromStdString(t->album().toCString(1));
    if(core->alumb == "" || QString(core->alumb).replace(" ", "") == ""){
        core->alumb = core->title;
    }
    core->genre = QString::fromStdString(t->genre().toCString(1));
    core->year = QString::number(t->year());

    //读取已毫秒为时长
    TagLib::AudioProperties* audio = f.audioProperties();
    int64_t time = audio->lengthInMilliseconds();
    core->endTime = time;
}

/*
*从文件中提取图片
*/
QImage ExtraLibrary::loadIndexCover(QString musicUrl)
{
    QImage img;
    //从文件中读取封面
    AVFormatContext *fmt_ctx = NULL;
    if (avformat_open_input(&fmt_ctx, musicUrl.toUtf8(), NULL, NULL)){
        char errbuf[1024];
        av_strerror(AVERROR_UNKNOWN, errbuf, sizeof(errbuf));
    }
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++){
        if (fmt_ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC){
            AVPacket pkt = fmt_ctx->streams[i]->attached_pic;
            //使用QImage读取完整图片数据
            img = QImage::fromData((uchar*)pkt.data, pkt.size);
            img = img.scaled(300, 300, Qt::KeepAspectRatioByExpanding);
        }
    }
    // 清理
    avformat_close_input(&fmt_ctx);

    return img;
}

QVector<double> ExtraLibrary::useFftw3(QVector<double> in, int N)
{
    QVector<double>out(N);
    // 创建一个FFTW计划（plan），它指定了如何计算DFT
    fftw_complex *in_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
    fftw_complex *out_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));

    // 将std::vector数据复制到fftw_complex数组中
    for (int i = 0; i < N; ++i) {
        in_ptr[i][0] = in[i];
        in_ptr[i][1] = 0.0;
    }

    // 创建计划并计算DFT
    fftw_plan plan = fftw_plan_dft_1d(N, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // 将结果从fftw_complex数组复制回std::vector
    for (int i = 0; i < N; ++i)
    {
        out[i] = sqrt(out_ptr[i][0]*out_ptr[i][0]+out_ptr[i][1]*out_ptr[i][1]);
    }

    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in_ptr);
    fftw_free(out_ptr);
    return out;
}

void formatConverByFFmpeg::logError(QString text){
    char error[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(r, error, AV_ERROR_MAX_STRING_SIZE);
    qDebug()<<text<<" "<<r<<":"<<error;

    closeAll();
}

void formatConverByFFmpeg::closeAll()
{
    avcodec_free_context(&decodeCtx);
    avcodec_free_context(&encodeCtx);
    avformat_free_context(inFmtCtx);
    avformat_free_context(outFmtCtx);
    if (!(outFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&outFmtCtx->pb);
    }
    av_packet_free(&inPak);
    av_packet_free(&outPak);
    av_frame_free(&inFrame);
    av_frame_free(&outFrame);
    swr_free(&swr);
}

bool formatConverByFFmpeg::toFlac(QString url){
    QString inUrl = url;
    QString suffix = url.split(".").last();
    QString outUrl = url.replace("."+suffix, ".flac");
    r = avformat_open_input(&inFmtCtx, inUrl.toUtf8(), nullptr, nullptr);
    if(r<0){
        logError("open input file fail");
        return false;
    }

    r = avformat_find_stream_info(inFmtCtx, nullptr);
    if(r<0){
        logError("no stream find");
        return false;
    }

    r = av_find_best_stream(inFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(r<0){
        logError("no audio stream find");
        return false;
    }
    inAudioStream = inFmtCtx->streams[r];

    //生成解码器
    const AVCodec *decodec = avcodec_find_decoder(inAudioStream->codecpar->codec_id);
    decodeCtx = avcodec_alloc_context3(decodec);
    avcodec_parameters_to_context(decodeCtx, inAudioStream->codecpar);
    r = avcodec_open2(decodeCtx, decodec, nullptr);
    if(r<0){
        logError("open decodec fail");
        return false;
    }

    //打开输出文件
    r = avformat_alloc_output_context2(&outFmtCtx, nullptr, nullptr, outUrl.toUtf8());
    if(r<0){
        logError("outUrlile open fail");
        return false;
    }
    //时长flac流
    outAudioStream = avformat_new_stream(outFmtCtx, nullptr);
    if(outAudioStream == NULL){
        logError("get new stream fail");
        return false;
    }

    const AVCodec *encodec = avcodec_find_encoder(AV_CODEC_ID_FLAC);
    encodeCtx = avcodec_alloc_context3(encodec);
    avcodec_parameters_to_context(encodeCtx, inAudioStream->codecpar);
    encodeCtx->codec_id = AV_CODEC_ID_FLAC;
    encodeCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    encodeCtx->ch_layout = decodeCtx->ch_layout;
    encodeCtx->sample_fmt = AV_SAMPLE_FMT_S16;
    encodeCtx->sample_rate = 44100;
    encodeCtx->bit_rate = 0;
    encodeCtx->time_base.den = encodeCtx->sample_rate;
    encodeCtx->time_base.num = 1;
    encodeCtx->bits_per_raw_sample = 16;
    r = avcodec_open2(encodeCtx, encodec, nullptr);
    if(r<0){
        logError("open encodec fail");
        return false;
    }
    avcodec_parameters_from_context(outAudioStream->codecpar, encodeCtx);

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

    inPak = av_packet_alloc();
    inFrame = av_frame_alloc();
    swr_alloc_set_opts2(&swr,
                        &encodeCtx->ch_layout, encodeCtx->sample_fmt, encodeCtx->sample_rate,
                        &decodeCtx->ch_layout, decodeCtx->sample_fmt, decodeCtx->sample_rate,
                        0, nullptr);

    int64_t pts = 0;

    while(av_read_frame(inFmtCtx, inPak) >= 0){
        inPak->stream_index = inAudioStream->index;
        r = avcodec_send_packet(decodeCtx, inPak);
        if(r<0){
            logError("send packet fail");
            return false;
        }

        while(avcodec_receive_frame(decodeCtx, inFrame) >= 0){
            outFrame = av_frame_alloc();
            outFrame->sample_rate = encodeCtx->sample_rate;
            outFrame->format = encodeCtx->sample_fmt;
            outFrame->ch_layout = encodeCtx->ch_layout;

            r = swr_convert_frame(swr, outFrame, inFrame);
            if(r<0){
                logError("swr fail");
                return false;
            }
            encodeCtx->frame_size = outFrame->nb_samples;

            r = avcodec_send_frame(encodeCtx, outFrame);
            if(r<0){
                logError("send frame fail");
                return false;
            }

            outPak = av_packet_alloc();
            while(avcodec_receive_packet(encodeCtx, outPak) >= 0){
                r = av_interleaved_write_frame(outFmtCtx, outPak);
                if(r<0){
                    logError("write frame fail");
                    return false;
                }
            }
            av_packet_unref(outPak);
            av_frame_unref(outFrame);
        }
    }

    //刷新编码器缓冲区
    avcodec_send_frame(encodeCtx, nullptr);
    while (avcodec_receive_packet(encodeCtx, outPak) >= 0) {
        r = av_interleaved_write_frame(outFmtCtx, outPak);
        if (r < 0) {
            logError("write frame fail");
            return false;
        }
        av_packet_unref(outPak);
    }
    av_packet_free(&outPak);

    r = av_write_trailer(outFmtCtx);
    if(r<0){
        logError("write trailer fial");
        return false;
    }

    closeAll();
    return true;
}

bool formatConverByFFmpeg::toMp3(QString url){
    QString inUrl = url;
    QString suffix = url.split(".").last();
    QString outUrl = url.replace("."+suffix, ".mp3");
    r = avformat_open_input(&inFmtCtx, inUrl.toUtf8(), nullptr, nullptr);
    if(r<0){
        logError("open input file fail");
        return false;
    }

    r = avformat_find_stream_info(inFmtCtx, nullptr);
    if(r<0){
        logError("no stream find");
        return false;
    }

    r = av_find_best_stream(inFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if(r<0){
        logError("no audio stream find");
        return false;
    }
    inAudioStream = inFmtCtx->streams[r];

    //生成解码器
    const AVCodec *decodec = avcodec_find_decoder(inAudioStream->codecpar->codec_id);
    decodeCtx = avcodec_alloc_context3(decodec);
    avcodec_parameters_to_context(decodeCtx, inAudioStream->codecpar);
    r = avcodec_open2(decodeCtx, decodec, nullptr);
    if(r<0){
        logError("open decodec fail");
        return false;
    }

    //打开输出文件
    r = avformat_alloc_output_context2(&outFmtCtx, nullptr, nullptr, outUrl.toUtf8());
    if(r<0){
        logError("outUrlile open fail");
        return false;
    }
    //时长flac流
    outAudioStream = avformat_new_stream(outFmtCtx, nullptr);
    if(outAudioStream == NULL){
        logError("get new stream fail");
        return false;
    }

    const AVCodec *encodec = avcodec_find_encoder(AV_CODEC_ID_MP3);
    encodeCtx = avcodec_alloc_context3(encodec);
    avcodec_parameters_to_context(encodeCtx, inAudioStream->codecpar);
    encodeCtx->codec_id = AV_CODEC_ID_MP3;
    encodeCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    encodeCtx->sample_fmt = AV_SAMPLE_FMT_S32P;
    encodeCtx->sample_rate = 44100;
    encodeCtx->bit_rate = 320000;
    r = avcodec_open2(encodeCtx, encodec, nullptr);
    if(r<0){
        logError("open encodec fail");
        return false;
    }
    avcodec_parameters_from_context(outAudioStream->codecpar, encodeCtx);

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

    inPak = av_packet_alloc();
    inFrame = av_frame_alloc();
    swr_alloc_set_opts2(&swr,
                        &encodeCtx->ch_layout, encodeCtx->sample_fmt, encodeCtx->sample_rate,
                        &decodeCtx->ch_layout, decodeCtx->sample_fmt, decodeCtx->sample_rate,
                        0, nullptr);

    int64_t pts = 0;

    while(av_read_frame(inFmtCtx, inPak) >= 0){
        //判断是否等于音频流,不是则跳过
        if(inPak->stream_index != inAudioStream->index){
            av_packet_unref(inPak);
            continue;
        }

        //解码数据
        r = avcodec_send_packet(decodeCtx, inPak);
        if(r<0){
            logError("send packet fail");
            return false;
        }

        //接受解码之后的数据
        while(avcodec_receive_frame(decodeCtx, inFrame) >= 0){
            outFrame = av_frame_alloc();
            outFrame->sample_rate = encodeCtx->sample_rate;
            outFrame->format = encodeCtx->sample_fmt;
            outFrame->ch_layout = encodeCtx->ch_layout;

            r = swr_convert_frame(swr, outFrame, inFrame);
            if(r<0){
                logError("swr fail");
                return false;
            }

            //处理不同长度的编码，长样本截断，短样本变化编码长度
            int pos = 0;
            while(pos < outFrame->nb_samples){
                AVFrame *frame = av_frame_alloc();
                frame->sample_rate = encodeCtx->sample_rate;
                frame->ch_layout = encodeCtx->ch_layout;
                frame->format = encodeCtx->sample_fmt;
                frame->pts = av_rescale_q(inFrame->pts, inFrame->time_base, encodeCtx->time_base);
                //得到实际帧数并处理短帧
                int sampleNumber = pos + 1152 >= outFrame->nb_samples ? outFrame->nb_samples - pos : 1152;
                int length = av_get_bytes_per_sample((AVSampleFormat)outFrame->format);
                frame->nb_samples = sampleNumber;
                encodeCtx->frame_size = sampleNumber;
                av_frame_get_buffer(frame, 0);

                for(int i=0; i<frame->ch_layout.nb_channels; i++){
                    memcpy(frame->data[i],
                           outFrame->data[i] + pos * length,
                           length * sampleNumber
                           );
                }
                pos += sampleNumber;

                r = avcodec_send_frame(encodeCtx, outFrame);
                if(r<0){
                    logError("send frame fail");
                    return false;
                }

                outPak = av_packet_alloc();
                while(avcodec_receive_packet(encodeCtx, outPak) >= 0){
                    r = av_interleaved_write_frame(outFmtCtx, outPak);
                    if(r<0){
                        logError("write frame fail");
                        return false;
                    }
                }
                av_packet_unref(outPak);
                av_frame_free(&frame);
            }
            av_frame_unref(outFrame);
        }
    }


    //刷新编码器缓冲区
    outPak = av_packet_alloc();
    avcodec_send_frame(encodeCtx, NULL);
    while (avcodec_receive_packet(encodeCtx, outPak) >= 0) {
        r = av_interleaved_write_frame(outFmtCtx, outPak);
        if (r < 0) {
            logError("write frame fail");
            return false;
        }
    }
    av_packet_unref(outPak);

    r = av_write_trailer(outFmtCtx);
    if(r<0){
        logError("write trailer fial");
        return false;
    }

    closeAll();
    return true;
}
