//
// Created by changzhi on 2026/2/27.
//

#include "FrequencySpectrum.h"

#include <algorithm>
#include <cmath>
#include <qmath.h>
#include <QVector>
#include "fftw3.h"
#include  <QThread>
#include <QDebug>

FrequencySpectrum::FrequencySpectrum()
    : fftwSize(2048) {
    // 固定 FFT 长度并复用工作区和计划，避免每个音频块重复创建 FFTW 资源。
    m_fftInput = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * fftwSize));
    m_fftOutput = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * fftwSize));
    if (m_fftInput != nullptr && m_fftOutput != nullptr) {
        m_fftPlan = fftw_plan_dft_1d(fftwSize, m_fftInput, m_fftOutput, FFTW_FORWARD, FFTW_ESTIMATE);
    }

    m_thread = new QThread;
    m_thread->start();
    moveToThread(m_thread);
}

FrequencySpectrum::~FrequencySpectrum() {
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
    if (m_fftPlan != nullptr) {
        fftw_destroy_plan(m_fftPlan);
    }
    fftw_free(m_fftInput);
    fftw_free(m_fftOutput);
}

inline void FrequencySpectrum::mixChannels(const int frameCount, const int channelCount, QVector<double> &data) {
    // 将多声道音频样本（例如立体声左,右声道）混合为单声道

    for (int i = 0; i < frameCount; ++i) {
        double    sum  = 0.0;
        const int base = i * channelCount;
        for (int j = 0; j < channelCount; ++j) {
            sum += data[base + j];
        }
        data[i] = sum / channelCount; // 平均幅度
    }
    data.resize(frameCount);
}

inline void FrequencySpectrum::applyHannWindow() {
    const int windowSize = data.size();
    if (windowSize != m_windowSize) {
        m_hannWindow.resize(windowSize);
        windowSum = 0.0;
        for (int i = 0; i < windowSize; ++i) {
            const double window = 0.5 * (1 - std::cos(2 * M_PI * i / (windowSize - 1)));
            m_hannWindow[i] = window;
            windowSum += window;
        }
        m_windowSize = windowSize;
    }

    for (int i = 0; i < windowSize; ++i) {
        data[i] *= m_hannWindow[i];
    }
}

inline void FrequencySpectrum::performFFT() {
    if (m_fftPlan == nullptr) {
        data.clear();
        return;
    }

    for (int i = 0; i < fftwSize; ++i) {
        m_fftInput[i][0] = data[i];
        m_fftInput[i][1] = 0.0;
    }

    fftw_execute(m_fftPlan);

    // 计算幅度谱
    const int useLength = fftwSize / 2;
    for (int i = 0; i < useLength; ++i) {
        data[i] = std::sqrt(
            m_fftOutput[i][0] * m_fftOutput[i][0] +
            m_fftOutput[i][1] * m_fftOutput[i][1]
        );
    }
    data.resize(useLength);
}

inline void FrequencySpectrum::normalizeData() {
    // 按 Hann 窗的增益还原单边幅度谱，并将 dBFS 映射到显示范围。
    constexpr double floorDb = -72.0;
    const double scale = windowSum > 0.0 ? 2.0 / windowSum : 0.0;
    for (double &val: data) {
        const double amplitude = val * scale;
        const double db = amplitude > 0.0 ? 20.0 * std::log10(amplitude) : floorDb;
        val = std::clamp((db - floorDb) / -floorDb, 0.0, 1.0);
    }
}

inline void FrequencySpectrum::smoothData() {
    if (data.empty() || !std::isfinite(data[0])) {
        return;
    }

    // 对齐数据长度
    if (lastData.size() != data.size()) {
        lastData.resize(data.size());
    }

    //时间平滑函数
    for (int i = 0; i < data.size(); i++) {
        if (data[i] < lastData[i]) {
            constexpr double smoothConstantDown = 0.2;
            data[i]                             = smoothConstantDown * data[i] + (1.0 - smoothConstantDown) * lastData[i];
        } else {
            constexpr double smoothConstantUp = 0.8;
            data[i]                           = smoothConstantUp * data[i] + (1.0 - smoothConstantUp) * lastData[i];
        }
    }

    lastData = data;
}

inline void FrequencySpectrum::downsampleData() {
    // 用对数频率分箱覆盖可听频段，避免线性抽样丢失低频细节。
    constexpr int displayBins = 60;
    const int spectrumSize = data.size();
    if (spectrumSize == 0 || sampleRate <= 0 || fftwSize <= 0) {
        data.clear();
        return;
    }

    const int binCount = displayBins;
    const double maxFrequency = static_cast<double>(sampleRate) / 2.0;
    const double minFrequency = std::min(20.0, maxFrequency);
    const double ratio = maxFrequency / minFrequency;
    const double binWidth = static_cast<double>(sampleRate) / fftwSize;
    QVector<double> result(binCount);

    for (int i = 0; i < binCount; ++i) {
        const double lowFrequency = minFrequency * std::pow(ratio, static_cast<double>(i) / binCount);
        const double highFrequency = minFrequency * std::pow(ratio, static_cast<double>(i + 1) / binCount);
        const int firstBin = std::clamp(static_cast<int>(std::ceil(lowFrequency / binWidth)), 0, spectrumSize);
        const int lastBin = std::clamp(static_cast<int>(std::ceil(highFrequency / binWidth)), 0, spectrumSize);

        double peak = 0.0;
        if (firstBin == lastBin) {
            // 音频块较短时，一个 FFT 频点可能覆盖多个显示频段，按中心频率选最近频点。
            const double centerFrequency = std::sqrt(lowFrequency * highFrequency);
            const int minBin = spectrumSize > 1 ? 1 : 0;
            const int nearestBin = std::clamp(
                static_cast<int>(std::round(centerFrequency / binWidth)),
                minBin,
                spectrumSize - 1);
            peak = data[nearestBin];
        } else {
            // 有多个 FFT 频点时取峰值，避免窄带音调被区间平均压低。
            for (int bin = firstBin; bin < lastBin; ++bin) {
                peak = std::max(peak, data[bin]);
            }
        }
        result[i] = peak;
    }
    data = std::move(result);
}

void FrequencySpectrum::runSpectrum(const QAudioBuffer &buffer) {
    const int frameCount   = static_cast<int>(buffer.frameCount());
    const int channelCount = buffer.format().channelCount();
    sampleRate             = buffer.format().sampleRate();

    if (frameCount <= 1 || channelCount <= 0 || sampleRate <= 0) {
        return;
    }

    originalData = getOriginalData(buffer);
    if (originalData.size() != frameCount * channelCount) {
        return;
    }
    mixChannels(frameCount, channelCount, originalData);

    data = originalData;
    if (data.size() > fftwSize) {
        data.resize(fftwSize);
    }

    applyHannWindow();
    data.resize(fftwSize);

    // 计算fft 得到幅度
    performFFT();

    // 降采样
    downsampleData();

    normalizeData();

    smoothData();

    emit dataFinished(data);
}

QVector<double> FrequencySpectrum::getOriginalData(const QAudioBuffer &buffer) {
    const auto      format       = buffer.format();
    const int       totalSamples = static_cast<int>(buffer.sampleCount());
    QVector<double> samples(totalSamples);

    switch (format.sampleFormat()) {
        case QAudioFormat::UInt8: {
            const auto *data = buffer.constData<quint8>();
            for (int i = 0; i < totalSamples; ++i) {
                // 8位无符号：范围 0~255，归一化到 -1.0~1.0
                samples[i] = (data[i] / 128.0) - 1.0;
            }
            break;
        }

        case QAudioFormat::Int16: {
            const auto *data = buffer.constData<qint16>();
            for (int i = 0; i < totalSamples; ++i) {
                // 16位有符号：范围 -32768~32767，除以 32768 归一化
                samples[i] = data[i] / 32768.0;
            }
            break;
        }

        case QAudioFormat::Int32: {
            const auto *data = buffer.constData<qint32>();
            for (int i = 0; i < totalSamples; ++i) {
                // 32位有符号：范围 -2147483648~2147483647，除以 2147483648 归一化
                samples[i] = data[i] / 2147483648.0;
            }
            break;
        }

        case QAudioFormat::Float: {
            const auto *data = buffer.constData<float>();
            for (int i = 0; i < totalSamples; ++i) {
                // float 已经是归一化的 -1.0~1.0
                samples[i] = data[i];
            }
            break;
        }

        default:
            samples.resize(0);
            break;
    }

    return std::move(samples);
}
