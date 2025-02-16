#include "extralibrary.h"
#include "popupdata.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "fftw3.h"
}

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

//将音乐文件转化为mp3
int ExtraLibrary::musicFileChangeMP3(QString url)
{
    QString inFile = url;
    QString suffix = url.split(".").last();
    if(suffix == "mp3"){
        return -1;
    }
    QString onFile = url.replace("."+suffix, ".mp3");
    AVFormatContext *inFmtCtx = NULL, *outFmtCtx = NULL;

    if(0 > avformat_open_input(&inFmtCtx, inFile.toUtf8(), NULL, NULL)){
        qDebug()<<"输入文件打开失败";
        avformat_close_input(&inFmtCtx);
        return -1;
    }

    if(0 > avformat_find_stream_info(inFmtCtx, NULL)){
        qDebug()<<"找不到输入流";
        avformat_close_input(&inFmtCtx);
        return -1;
    }

    if(0 > avformat_alloc_output_context2(&outFmtCtx, NULL, NULL, onFile.toUtf8())){
        qDebug()<<"输出文件打开失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        return -1;
    }

    int aimStream = av_find_best_stream(inFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    AVStream *inStream = inFmtCtx->streams[aimStream];
    AVStream *outStream = avformat_new_stream(outFmtCtx, NULL);

    //生成解码器
    const AVCodec *dncode = avcodec_find_decoder(inStream->codecpar->codec_id);
    if(!dncode){
        qDebug()<<"找不到解码器";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        return -1;
    }
    AVCodecContext *dncodeCtx = avcodec_alloc_context3(dncode);
    avcodec_parameters_to_context(dncodeCtx, inStream->codecpar);
    if(avcodec_open2(dncodeCtx, dncode, NULL) < 0){//打开解码
        qDebug()<<"解码器打开失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }

    //生成mp3编码器
    const AVCodec *encode = avcodec_find_encoder(AV_CODEC_ID_MP3);
    if(!encode){
        qDebug()<<"找不到编码器";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }
    AVCodecContext *encodeCtx = avcodec_alloc_context3(encode);
    avcodec_parameters_to_context(encodeCtx, inStream->codecpar);
    encodeCtx->codec_id = AV_CODEC_ID_MP3;
    encodeCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    encodeCtx->sample_fmt = AV_SAMPLE_FMT_S32P;
    encodeCtx->sample_rate = 44100;
    encodeCtx->bit_rate = 320000;
    if(0 > avcodec_parameters_from_context(outStream->codecpar, encodeCtx)){
        qDebug()<<"输入流拷贝数据失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&encodeCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }
    if(avcodec_open2(encodeCtx, encode, NULL) < 0){//打开编码
        qDebug()<<"编码器打开失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&encodeCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }

    if(0 > avio_open(&outFmtCtx->pb, onFile.toUtf8(), AVIO_FLAG_WRITE)){
        qDebug()<<"输出文件打开失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&encodeCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }

    //拷贝元数据
    AVDictionaryEntry* tag = nullptr;
    while ((tag = av_dict_get(inFmtCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        av_dict_set(&outFmtCtx->metadata, tag->key, tag->value, 0);
    }

    if(0 > avformat_write_header(outFmtCtx, NULL)){
        qDebug()<<"写入文件头失败";
        avformat_close_input(&inFmtCtx);
        avformat_close_input(&outFmtCtx);
        avcodec_free_context(&encodeCtx);
        avcodec_free_context(&dncodeCtx);
        return -1;
    }

    //解码再编码
    AVPacket *iapk = av_packet_alloc();
    AVFrame *inFrame = av_frame_alloc();
    SwrContext *swr = NULL;
    swr_alloc_set_opts2(&swr,
                        &dncodeCtx->ch_layout, dncodeCtx->sample_fmt, dncodeCtx->sample_rate,
                        &encodeCtx->ch_layout, encodeCtx->sample_fmt, encodeCtx->sample_rate,
                        0, NULL);

    while(av_read_frame(inFmtCtx, iapk) >= 0){
        avcodec_send_packet(dncodeCtx, iapk);
        while(avcodec_receive_frame(dncodeCtx, inFrame) >= 0){//接受解码后的数据
            AVFrame *outFrame = av_frame_alloc();
            outFrame->sample_rate = encodeCtx->sample_rate;
            outFrame->ch_layout = encodeCtx->ch_layout;
            outFrame->format = encodeCtx->sample_fmt;
            swr_convert_frame(swr, outFrame, inFrame);//重采样

            int pos = 0;
            while(pos < outFrame->nb_samples){
                AVFrame *frame = av_frame_alloc();
                frame->sample_rate = encodeCtx->sample_rate;
                frame->ch_layout = encodeCtx->ch_layout;
                frame->format = encodeCtx->sample_fmt;
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

                avcodec_send_frame(encodeCtx, frame);

                AVPacket *oapk = av_packet_alloc();
                while(avcodec_receive_packet(encodeCtx, oapk) >= 0){//接受编码后的数据
                    if(av_interleaved_write_frame(outFmtCtx, oapk) < 0){
                        qDebug()<<"写入数据帧失败";
                        break;
                    }
                    av_packet_unref(oapk);
                }
            }
            av_frame_unref(outFrame);
        }
    }
    av_packet_unref(iapk);//消除

    av_write_trailer(outFmtCtx);//写入文件尾

    //清除控件
    avformat_close_input(&inFmtCtx);
    avformat_close_input(&outFmtCtx);
    avcodec_free_context(&encodeCtx);
    avcodec_free_context(&dncodeCtx);
    return 1;
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
