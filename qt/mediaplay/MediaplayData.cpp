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
    m_frequencySpectrum = new FrequencySpectrum;//音频计算

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    m_dataActive = DataActive::getInstance();

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, m_frequencySpectrum, &FrequencySpectrum::runSpectrum);
    connect(m_frequencySpectrum, &FrequencySpectrum::dataFinished, this, [this](QVector<double> data) {
        m_allSamples = data;
        emit bufferSampleChanged();
    });

    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 time) {
        updateAudioOutPut();
    });
}

MediaPlayData::~MediaPlayData() {
    m_player->deleteLater();
    m_audioOutput->deleteLater();
    m_bufferOutput->deleteLater();
    m_frequencySpectrum->deleteLater();
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
