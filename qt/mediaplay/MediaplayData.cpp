#include "MediaplayData.h"

#include <iostream>

#include <QAudioOutput>
#include <QThread>

MediaPlayData::MediaPlayData() {
    m_player            = new QMediaPlayer;       //播放设备
    m_audioOutput       = new QAudioOutput;       //音频输出
    m_bufferOutput      = new QAudioBufferOutput; //缓冲区输出
    m_frequencySpectrum = new FrequencySpectrum;  //音频计算

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, m_frequencySpectrum,
            &FrequencySpectrum::runSpectrum);
    connect(m_frequencySpectrum, &FrequencySpectrum::dataFinished, this, [this](const QVector<double> &data) {
        m_allSamples = data;
        emit bufferSampleChanged();
    });

    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 time) {
        updateAudioOutPut();
    });
}

MediaPlayData::~MediaPlayData() {
    m_player->stop();
    disconnect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, m_frequencySpectrum,
               &FrequencySpectrum::runSpectrum);
    disconnect(m_frequencySpectrum, &FrequencySpectrum::dataFinished, this, nullptr);
    disconnect(m_player, &QMediaPlayer::positionChanged, this, nullptr);
    if (m_frequencySpectrum != nullptr && m_frequencySpectrum->thread() != nullptr) {
        m_frequencySpectrum->thread()->quit();
        m_frequencySpectrum->thread()->wait();
    }
    delete m_frequencySpectrum;
    delete m_bufferOutput;
    delete m_audioOutput;
    delete m_player;
}

void MediaPlayData::clearData() {
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
