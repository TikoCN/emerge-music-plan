//
// Created by changzhi on 2026/2/27.
//

#include "FrequencySpectrum.h"

#include <qmath.h>
#include <QVector>
#include "fftw3.h"
#include  <QThread>
#include <QDebug>

FrequencySpectrum::FrequencySpectrum()
    : fftwSize(2048) {
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
        data[i] = sum / channelCount; // 平均幅度
    }
    data.resize(frameCount);
}

inline void FrequencySpectrum::applyHannWindow() {
    // 加窗
    const int N = static_cast<int>(data.size());
    for (int i = 0; i < N; ++i) {
        const double window = 0.5 * (1 - std::cos(2 * M_PI * i / (N - 1)));
        data[i] *= window; // 将原始数据乘以窗系数
    }
}

inline void FrequencySpectrum::performFFT() {
    //计算傅里叶变换
    auto *in_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * fftwSize));
    auto *out_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * fftwSize));

    // 将vector数据复制到fftw_complex数组中
    for (int i = 0; i < fftwSize; ++i) {
        in_ptr[i][0] = data[i];
        in_ptr[i][1] = 0.0;
    }

    // 创建计划并计算DFT
    // ReSharper disable once CppLocalVariableMayBeConst
    fftw_plan plan = fftw_plan_dft_1d(fftwSize, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // 计算幅度谱
    const int useLength = fftwSize / 2;
    for (int i = 0; i < useLength; ++i) {
        data[i] = std::sqrt(
            out_ptr[i][0] * out_ptr[i][0] +
            out_ptr[i][1] * out_ptr[i][1]
        );
    }
    data.resize(useLength);

    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in_ptr);
    fftw_free(out_ptr);
}

inline void FrequencySpectrum::normalizeData() {
    //归一化
    for (double &val: data) {
        val /= fftwSize * 0.09;
        if (val > 0.3) {
            val = val * 0.5 + 0.1;
        }
        val = std::max(val, 0.0);
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
            data[i] = smoothConstantDown * data[i] + (1.0 - smoothConstantDown) * lastData[i];
        } else {
            constexpr double smoothConstantUp = 0.8;
            data[i] = smoothConstantUp * data[i] + (1.0 - smoothConstantUp) * lastData[i];
        }
    }

    // 当前平滑
    QVector<double> smooth(data.size());
    smooth[0] = data[0];
    for (int i = 1; i < data.size(); ++i) {
        smooth[i] = smooth[i - 1] + (data[i] - smooth[i - 1]) * 0.8;
        data[i] = smooth[i];
    }
    lastData = data;
}

inline void FrequencySpectrum::downsampleData() {
    // 降采样
    unsigned int lastId = 0;
    double hz = 0;
    const double max = static_cast<double>(sampleRate) / fftwSize;
    const double aim = std::min(60, static_cast<int>(data.length()));
    const double cell = max / aim;

    for (int i = 0; i < aim; ++i) {
        const double aimN = i * cell;
        hz = std::exp(aimN);
        const auto n = static_cast<unsigned int>(hz * static_cast<double>(fftwSize) / static_cast<double>(sampleRate));
        if (lastId != n && n < data.size()) {
            data[i] = data[n];
            lastId = n;
        }
    }
    data.resize(static_cast<int>(aim));
}

void FrequencySpectrum::runSpectrum(const QAudioBuffer &buffer) {
    const int frameCount = static_cast<int>(buffer.frameCount());
    const int channelCount = buffer.format().channelCount();
    sampleRate = buffer.format().sampleRate();

    originalData = getOriginalData(buffer);
    mixChannels(frameCount, channelCount, originalData);

    const int len = static_cast<int>(originalData.length());

    fftwSize = 1;
    while (fftwSize < len) {
        fftwSize <<= 1;;
    }

    originalData.resize(fftwSize);
    data = originalData;

    applyHannWindow();

    // 计算fft 得到幅度
    performFFT();

    // 降采样
    downsampleData();

    normalizeData();

    smoothData();

    emit dataFinished(data);
}

QVector<double> FrequencySpectrum::getOriginalData(const QAudioBuffer &buffer) {
    const auto format = buffer.format();
    const int totalSamples = static_cast<int>(buffer.sampleCount());
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