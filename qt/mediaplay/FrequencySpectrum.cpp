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
        data[i] = sum / channelCount;  // 平均幅度
    }
    data.resize(frameCount);
}

inline void FrequencySpectrum::applyHannWindow() {
    // 加窗
    const int N = static_cast<int>(data.size());                 // 获取数据长度
    for (int i = 0; i < N; ++i) {               // 遍历每个样本点
        const double window = 0.5 * (1 - std::cos(2 * M_PI * i / (N - 1)));
        data[i] *= window;                       // 将原始数据乘以窗系数
    }
}

inline void FrequencySpectrum::performFFT() {

    //计算傅里叶变换
    // 创建一个FFTW计划
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
    for (int i = 1; i < useLength; ++i) {
        data[i] = std::sqrt(
            out_ptr[i][0] * out_ptr[i][0] +
            out_ptr[i][1] * out_ptr[i][1]
        );
    }
    data.remove(20);
    data.resize(useLength - 20);

    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in_ptr);
    fftw_free(out_ptr);
}

inline void FrequencySpectrum::normalizeData() {

    //归一化
    double maxVal = 0.0;
    double minVal = 0.0;
    for (const double i : data) {
        maxVal = std::max(i, maxVal);
        minVal = std::min(i, minVal);
    }
    for (double & val : data) {
        //val = std::abs(val / maxVal);
        val = (val - minVal) / (maxVal - minVal);
        val = std::max(val, 0.0);
    }
}

inline void FrequencySpectrum::applyWeight() {
    if (data.empty() || 0 == sampleRate) return;

    const auto len = static_cast<int>(data.size());

    // 转换为dB

    for (int i = 0; i < len; ++i) {
        data[i] = 20.0 * log10(data[i] / len + 1e-10);
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
        }
    }
    lastData = data;
}

inline void FrequencySpectrum::downsampleData() {
    // 降采样
    int i = 0, hz = 0;
    double max = fftwSize / fftwSize;
    double aim = std::min(120, (int)data.length());
    double cell = max / aim;

    for (int i = 0; i < aim; ++i) {
        double aimN = i * cell;
        hz = std::exp(aimN);
        const int n = hz * fftwSize / sampleRate;
        if (n < data.size())
            data[i] = data[n];
    }
    data.resize(aim);

}

void FrequencySpectrum::transform() {
    QVector<double> input = data;

    QVector<double> result;
    const int n = static_cast<int>(input.size());

    // 逆序收集偶数索引元素（从最后一个偶数索引开始）
    for (int i = (n % 2 == 0 ? n - 2 : n - 1); i >= 0; i -= 2) {
        result.append(input[i]);
    }
    // 顺序收集奇数索引元素
    for (int i = 1; i < n; i += 2) {
        result.append(input[i]);
    }
    data = std::move(result);
}

void FrequencySpectrum::runSpectrum(const QAudioBuffer &buffer) {
    const int frameCount = static_cast<int>(buffer.frameCount());
    const int channelCount = buffer.format().channelCount();
    sampleRate = buffer.format().sampleRate();

    int cell = 1;

    QVector<double> cache = getOriginalData(buffer);
    mixChannels(frameCount, channelCount, cache);

    originalData.append(std::move(cache));
    if (originalData.length() < fftwSize) {
        return;
    }

    data = std::move(QVector<double>(originalData.begin(), originalData.begin() + fftwSize));
    originalData.erase(originalData.begin(), originalData.begin() + fftwSize);

    applyHannWindow();

    // 计算fft 得到幅度
    performFFT();

    // 降采样
    downsampleData();

    // // 计算dp
    applyWeight();

    // 整体偏移
    transform();

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
        const quint8 *data = buffer.constData<quint8>();
        for (int i = 0; i < totalSamples; ++i) {
            // 8位无符号：范围 0~255，归一化到 -1.0~1.0
            samples[i] = (data[i] / 128.0) - 1.0;
        }
        break;
    }

    case QAudioFormat::Int16: {
        const qint16 *data = buffer.constData<qint16>();
        for (int i = 0; i < totalSamples; ++i) {
            // 16位有符号：范围 -32768~32767，除以 32768 归一化
            samples[i] = data[i] / 32768.0;
        }
        break;
    }

    case QAudioFormat::Int32: {
        const qint32 *data = buffer.constData<qint32>();
        for (int i = 0; i < totalSamples; ++i) {
            // 32位有符号：范围 -2147483648~2147483647，除以 2147483648 归一化
            samples[i] = data[i] / 2147483648.0;
        }
        break;
    }

    case QAudioFormat::Float: {
        const float *data = buffer.constData<float>();
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
