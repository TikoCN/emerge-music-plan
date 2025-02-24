#include "extralibrary.h"
#include "popupdata.h"

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

