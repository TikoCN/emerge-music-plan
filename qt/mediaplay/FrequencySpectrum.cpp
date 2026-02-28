//
// Created by changzhi on 2026/2/27.
//

#include "FrequencySpectrum.h"

#include <qmath.h>
#include <QVector>
#include "fftw3.h"
#include  <QThread>
#include <QDebug>

FrequencySpectrum::FrequencySpectrum() {
    m_thread = new QThread;
    m_thread->start();
    moveToThread(m_thread);
}

FrequencySpectrum::~FrequencySpectrum() {
    m_thread->quit();
    m_thread->deleteLater();
}

inline void FrequencySpectrum::mixChannels(const int frameCount, const int channelCount, QVector<double> &data) {
    // 将多声道音频样本（例如立体声左,右声道）混合为单声道

    for (int i = 0; i < frameCount; ++i) {
        double sum = 0.0;
        const int base = i * channelCount;
        for (int j = 0; j < channelCount; ++j) {
            sum += data[base + j];
        }
        data[i] = sum / channelCount;  // 平均幅度
    }
    data.resize(frameCount);
}

inline void FrequencySpectrum::applyHannWindow(QVector<double> &data) {
    // 加窗
    const int N = static_cast<int>(data.size());                 // 获取数据长度
    for (int i = 0; i < N; ++i) {               // 遍历每个样本点
        const double window = 0.5 * (1 - std::cos(2 * M_PI * i / (N - 1)));
        data[i] *= window;                       // 将原始数据乘以窗系数
    }
}

inline void FrequencySpectrum::performFFT(int frameCount, int sampleRate,  QVector<double> &data) {
    //计算傅里叶变换
    // 创建一个FFTW计划
    auto *in_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * frameCount));
    auto *out_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * frameCount));

    // 将vector数据复制到fftw_complex数组中
    for (int i = 0; i < frameCount; ++i) {
        in_ptr[i][0] = data[i];
        in_ptr[i][1] = 0.0;
    }

    // 创建计划并计算DFT
    // ReSharper disable once CppLocalVariableMayBeConst
    fftw_plan plan = fftw_plan_dft_1d(frameCount, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // 计算幅度谱并转换为dB
    const int useLength = frameCount / 2;
    for (int i = 0; i < useLength; ++i) {
        data[i] = std::sqrt(
            out_ptr[i][0] * out_ptr[i][0] +
            out_ptr[i][1] * out_ptr[i][1]
        );
    }
    data.resize(useLength);

    // // 转换为dB
    // const double maxMag = *std::max_element(data.begin(), data.end());
    // for (int i = 0; i < useLength; ++i) {
    //     data[i] = 20.0 * log10(data[i] / maxMag + 1e-10);
    // }

    for (int i = 0; i < useLength; ++i) {
        // 频率 = 频率箱 × (采样率 / FFT大小)
        double frequency = static_cast<double>(i) * sampleRate / frameCount;

        // 避免 0Hz 处理
        if (frequency < 1.0) frequency = 1.0;

        // A权重公式
        const double f2 = frequency * frequency;
        const double f4 = f2 * f2;

        const double numerator = 12194.217 * 12194.217 * f4;
        const double denominator = (f2 + 20.599997 * 20.599997) *
                            (f2 + 107.65265 * 107.65265) *
                            (f2 + 737.86223 * 737.86223) *
                            (f2 + 12194.217 * 12194.217);

        const double aWeight = 20.0 * std::log10(numerator / denominator) + 2.0;
        data[i] += aWeight; // 加到dB值上
    }

    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in_ptr);
    fftw_free(out_ptr);
}

inline void FrequencySpectrum::normalizeData(QVector<double> &data) {
    //归一化
    double maxVal = 0.0;
    double minVal = 0.0;
    for (const double i : data) {
        maxVal = std::max(i, maxVal);
        minVal = std::min(i, minVal);
    }
    for (double & val : data) {
        val = (val - minVal) / (maxVal - minVal);
        val = std::max(val, 0.0);
    }
}

inline void FrequencySpectrum::smoothData(QVector<double> &data) {
    // 对齐数据长度
    QVector<double> cache = data;
    if (lastData.size() != cache.size()) {
        lastData.resize(cache.size());
    }

    //时间平滑函数
    for (int i = 0; i < cache.size(); i++) {
        if (std::isfinite(cache[i])) {
            //判断是不是有理数
            if (cache[i] >= lastData[i]) {
                constexpr double smoothConstantUp = 0.8;
                cache[i] = smoothConstantUp * cache[i] + (1.0 - smoothConstantUp) * lastData[i];
            } else {
                constexpr double smoothConstantDown = 0.08;
                cache[i] = smoothConstantDown * cache[i] + (1.0 - smoothConstantDown) * lastData[i];
            }
        }
    }
    lastData = data;
    data = cache;
}

inline void FrequencySpectrum::downsampleData(QVector<double> &data) {
    // 降采样
    const int aim = static_cast<int>(qMin(data.size(), 120));
    const int cell = static_cast<int>(data.size() / aim);
    for (int i = 0; i < aim; i++) {
        double max = 0;
        const int basePos = i * cell;
        for (int j = 0; j < cell && basePos + j < data.size(); j++) {
            max = std::max(max, data[basePos + j]);
        }
        data[i] = max;
    }
    data.resize(aim);
}

void FrequencySpectrum::runSpectrum(const QAudioBuffer &buffer) {
    const int frameCount = static_cast<int>(buffer.frameCount());
    const int sampleCount = static_cast<int>(buffer.sampleCount());
    const int channelCount = buffer.format().channelCount();
    const int sampleRate = buffer.format().sampleRate();

    const auto *samples = buffer.constData<qint16>();
    QVector<double> data(sampleCount);
    for (int i=0; i<sampleCount; i++) {
        data[i] = samples[i];
    }

    mixChannels(frameCount, channelCount, data);

    applyHannWindow(data);

    performFFT(frameCount, sampleRate, data);

    normalizeData(data);

    smoothData(data);

    downsampleData(data);

    emit dataFinished(data);
}
