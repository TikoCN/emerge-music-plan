//
// Created by changzhi on 2026/2/27.
//

#ifndef MUSICPLAY_FREQUENCYSPECTRUM_H
#define MUSICPLAY_FREQUENCYSPECTRUM_H
#include <QAudioBuffer>
#include <qcontainerfwd.h>


class FrequencySpectrum : public QObject {
    Q_OBJECT
public:
    FrequencySpectrum();
    ~FrequencySpectrum() override;

    // 混合通道
    static void mixChannels(int frameCount, int channelCount, QVector<double> &data);

    void applyHannWindow();

    void performFFT();

    void normalizeData();

    void applyWeight();

    void smoothData();

    void downsampleData();

    void transform();

    //计算音频
    void runSpectrum(const QAudioBuffer &buffer);

    static QVector<double> getOriginalData(const QAudioBuffer &buffer);

    signals:
    void dataFinished(QVector<double>);

private:
    QThread *m_thread;
    QVector<double> lastData;
    QVector<double> data;
    QVector<double> originalData;
    int sampleCount;
    int sampleRate;
    const int fftwSize;

};



#endif //MUSICPLAY_FREQUENCYSPECTRUM_H
