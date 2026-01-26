#ifndef MEDIAPLAYDATA_H
#define MEDIAPLAYDATA_H

#include <QMediaPlayer>
#include <QAudioBuffer>
#include <QAudioDevice>
#include <QAudioBufferOutput>
#include <QAudioOutput>

#include "basetool/basetool.h"
#include "datacore/dataactive.h"

class MediaPlayData : public QObject
{
    Q_OBJECT

protected:
    BaseTool *m_baseTool;
    DataActive *m_dataActive;

    QMediaPlayer *m_player;//播放设备
    QAudioOutput *m_audioOutput;//音频输出
    QAudioBufferOutput *m_bufferOutput;//缓冲区输出
    QVector<double> m_allSamples;//处理之后的音乐样本

    Q_PROPERTY(QMediaPlayer *player READ getPlayer CONSTANT)
    Q_PROPERTY(QAudioOutput *audioOutput READ getAudioOutput CONSTANT)
    Q_PROPERTY(QVector<double> allSamples READ getAllSamples CONSTANT)

public:

    explicit MediaPlayData(BaseTool *baseTool, DataActive *dataActive);

    //删除以及加载的数据
    Q_INVOKABLE void clearData();
    //更新播放设备
    void updateAudioOutPut() const;
    //计算音频
    void buildFrequencySpectrum(const QAudioBuffer& buffer);
    [[nodiscard]] QMediaPlayer *getPlayer() const;
    [[nodiscard]] QVector<double> getAllSamples() const;
    [[nodiscard]] QAudioOutput *getAudioOutput() const;

signals:

    //清空数据以及播放列表
    void finishClearData();
    //绘制音频波形
    void cppDrawLine(QVector<double>);
};

#endif // MEDIAPLAYDATA_H
