#include "MediaplayData.h"

#include <iostream>

#include "fftw3.h"
#include <QAudioOutput>

MediaPlayData::MediaPlayData(BaseTool *baseTool, DataActive *dataActive, TLog *loger, QObject *parent)
    : QObject(parent),
      m_baseTool(baseTool),
      m_dataActive(dataActive),
      m_loger(loger)
{
    m_player = new QMediaPlayer; //播放设备
    m_audioOutput = new QAudioOutput; //音频输出
    m_bufferOutput = new QAudioBufferOutput; //缓冲区输出

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    m_dataActive = DataActive::getInstance();

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &MediaPlayData::buildFrequencySpectrum);

    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 time) {
        updateAudioOutPut();
    });
}

void MediaPlayData::clearData() {
    m_lastSamples.clear();
    m_allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
}

void MediaPlayData::updateAudioOutPut() const {
    const auto *nowOut = new QAudioOutput;
    if (m_audioOutput->device().id() != nowOut->device().id()) {
        m_audioOutput->setDevice(nowOut->device());

        m_player->setAudioOutput(m_audioOutput);
    }
    delete nowOut;
}

QAudioOutput *MediaPlayData::getAudioOutput() const {
    return m_audioOutput;
}

QVector<double> MediaPlayData::getAllSamples() const {
    return m_allSamples;
}

QMediaPlayer *MediaPlayData::getPlayer() const {
    return m_player;
}

void MediaPlayData::buildFrequencySpectrum(const QAudioBuffer &buffer) {
    //得到所有音频样本
    const auto *samples = buffer.constData<qint16>();
    const int all = static_cast<int>(buffer.frameCount()); //帧数
    const int sample = static_cast<int>(buffer.sampleCount()); //样本数
    const int channel = buffer.format().channelCount();

    if (all != 0) {
        QVector<double> data(all);

        // 将多声道音频样本（例如立体声左,右声道）混合为单声道
        for (int i = 0; i < all; ++i) {
            double sum = 0.0;
            const int base = i * channel;
            for (int j = 0; j < channel; ++j) {
                sum += samples[base + j];
            }
            data[i] = sum / channel;  // 平均幅度
        }

        //计算傅里叶变换
        // 创建一个FFTW计划
        auto *in_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * all));
        auto *out_ptr = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * all));

        // 将std::vector数据复制到fftw_complex数组中
        for (int i = 0; i < all; ++i) {
            in_ptr[i][0] = data[i];
            in_ptr[i][1] = 0.0;
        }

        // 创建计划并计算DFT
        // ReSharper disable once CppLocalVariableMayBeConst
        fftw_plan plan = fftw_plan_dft_1d(all, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan);

        // 计算幅度谱
        const int useLength = all / 2;
        for (int i = 0; i < useLength; ++i) {
            data[i] = sqrt(out_ptr[i][0] * out_ptr[i][0] + out_ptr[i][1] * out_ptr[i][1]);
        }
        data.resize(useLength);
        // 清理
        fftw_destroy_plan(plan);
        fftw_free(in_ptr);
        fftw_free(out_ptr);

        //归一化
        for (const double i : data) {
            if (i > m_maxHeightValue) {
                m_maxHeightValue = i;
            }
            if (i < m_minHeightValue) {
                m_minHeightValue = i;
            }
        }
        for (double & i : data) {
            i = (i - m_minHeightValue) / (m_maxHeightValue - m_minHeightValue);
            i = i < 0 ? 0 : i;
        }

        // 对齐数据长度
        if (m_allSamples.size() != data.size()) {}
        m_allSamples.resize(data.size());

        //时间平滑函数
        for (int i = 0; i < data.size() && i < m_allSamples.size(); i++) {
            if (std::isfinite(data[i])) {
                //判断是不是有理数
                if (data[i] > m_allSamples[i]) {
                    constexpr double smoothConstantUp = 0.8;
                    m_allSamples[i] = smoothConstantUp * data[i] + (1 - smoothConstantUp) * m_allSamples[i];
                } else {
                    constexpr double smoothConstantDown = 0.08;
                    m_allSamples[i] = smoothConstantDown * data[i] + (1 - smoothConstantDown) * m_allSamples[i];
                }
            }
        }

        // 降采样
        const int aim = static_cast<int>(qMin(m_allSamples.size(), 120));
        const int cell = static_cast<int>(m_allSamples.size() / aim);
        data.resize(aim);
        for (int i = 0; i < aim; i++) {
            const int basePos = i * cell;
            if (basePos < m_allSamples.size()) {
                data[i] = 0;
                continue;
            }

            double max = m_allSamples[basePos];
            for (int j = 1; j < cell && basePos + j < m_allSamples.size(); j++) {
                if (max < m_allSamples[basePos + j]) {
                    max = m_allSamples[basePos + j];
                }
            }
            data[i] = max;
        }

        emit bufferSampleChanged();
    }
}
