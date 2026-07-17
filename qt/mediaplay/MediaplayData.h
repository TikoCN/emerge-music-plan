#ifndef MEDIAPLAYDATA_H
#define MEDIAPLAYDATA_H

#include <QAudioDevice>
#include <QAudioBufferOutput>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "datacore/dataactive.h"
#include "mediaplay/FrequencySpectrum.h"

class MediaPlayData : public QObject {
    Q_OBJECT

protected:
    FrequencySpectrum *m_frequencySpectrum;

    QMediaPlayer *      m_player;       //播放设备
    QAudioOutput *      m_audioOutput;  //音频输出
    QAudioBufferOutput *m_bufferOutput; //缓冲区输出
    QVector<double>     m_allSamples;   //处理之后的音乐样本

    Q_PROPERTY(QMediaPlayer *player READ getPlayer CONSTANT)
    Q_PROPERTY(QAudioOutput *audioOutput READ getAudioOutput CONSTANT)
    Q_PROPERTY(QVector<double> allSamples READ getAllSamples CONSTANT)

public:
    explicit MediaPlayData();
    ~MediaPlayData() override;

    //删除以及加载的数据
    Q_INVOKABLE void clearData();
    //更新播放设备
    void updateAudioOutPut() const;

    [[nodiscard]] QMediaPlayer *  getPlayer() const;
    [[nodiscard]] QVector<double> getAllSamples() const;
    [[nodiscard]] QAudioOutput *  getAudioOutput() const;

signals:
    //清空数据以及播放列表
    void finishClearData();
    //绘制音频波形
    void bufferSampleChanged();
};

#endif // MEDIAPLAYDATA_H
