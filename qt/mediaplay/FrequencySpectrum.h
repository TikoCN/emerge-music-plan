//
// Created by changzhi on 2026/2/27.
//

#ifndef MUSICPLAY_FREQUENCYSPECTRUM_H
#define MUSICPLAY_FREQUENCYSPECTRUM_H
#include <QAudioBuffer>
#include <QVector>
#include <qcontainerfwd.h>
#include <fftw3.h>


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

    void smoothData();

    void downsampleData();

    //计算音频
    void runSpectrum(const QAudioBuffer &buffer);

    static QVector<double> getOriginalData(const QAudioBuffer &buffer);

signals:
    void dataFinished(QVector<double>);

private:
    QThread *       m_thread;
    QVector<double> lastData;
    QVector<double> data;
    QVector<double> originalData;
    int             sampleCount{};
    int             sampleRate{};
    int             fftwSize;
    int             m_windowSize = 0;
    double          windowSum{};
    QVector<double> m_hannWindow;
    fftw_complex *  m_fftInput = nullptr;
    fftw_complex *  m_fftOutput = nullptr;
    fftw_plan       m_fftPlan = nullptr;
};


#endif //MUSICPLAY_FREQUENCYSPECTRUM_H
