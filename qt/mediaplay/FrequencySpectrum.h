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
    QVector<double> lastData;

    // 混合通道
    static void mixChannels(int frameCount, int channelCount, QVector<double> &data);

    static void applyHannWindow(QVector<double> &data);

    static void performFFT(int frameCount, int sampleRate, QVector<double> &data);

    static void normalizeData(QVector<double> &data);

    void smoothData(QVector<double> &data);

    static void downsampleData(QVector<double> &data);

    //计算音频
    void runSpectrum(const QAudioBuffer& buffer);

    signals:
    void dataFinished(QVector<double>);

private:
    QThread *m_thread;

};



#endif //MUSICPLAY_FREQUENCYSPECTRUM_H
