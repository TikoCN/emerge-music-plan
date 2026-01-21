#include "mediaplaydata.h"
#include "fftw3.h"


MediaPlayData::MediaPlayData(BaseTool *baseTool, DataActive *dataActive)
    :m_baseTool(baseTool), m_dataActive(dataActive)
{
    m_player = new QMediaPlayer;//播放设备
    m_audioOutput = new QAudioOutput;//音频输出
    m_bufferOutput = new QAudioBufferOutput;//缓冲区输出

    m_player->setAudioOutput(m_audioOutput);
    m_player->setAudioBufferOutput(m_bufferOutput);

    m_dataActive = DataActive::getInstance();

    connect(m_bufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &MediaPlayData::buildFrequencySpectrum);

    connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 time){
        updateAudioOutPut();
    });
}

void MediaPlayData::clearData()
{
    // m_lrcList.clear();
    // m_musicList.clear();
    m_allSamples.clear();

    //发送信号，表示完成
    emit finishClearData();
}

void MediaPlayData::updateAudioOutPut()
{
    QAudioOutput* nowOut = new QAudioOutput;
    if(m_audioOutput->device().id() != nowOut->device().id())
    {
        m_audioOutput->setDevice(nowOut->device());


        m_player->setAudioOutput(m_audioOutput);
    }
    delete nowOut;
}

QAudioOutput *MediaPlayData::getAudioOutput() const
{
    return m_audioOutput;
}

QVector<double> MediaPlayData::getAllSamples() const
{
    return m_allSamples;
}

QMediaPlayer *MediaPlayData::getPlayer() const
{
    return m_player;
}

void MediaPlayData::buildFrequencySpectrum(QAudioBuffer buffer)
{
    //得到所有音频样本
    const qint16* samples = buffer.constData<qint16>();
    int all = buffer.frameCount();//采样单元
    int sample = buffer.sampleCount();//总频道数

    if(all != 0){
        int alone = sample/all;
        QVector<double>data(all);

        for (int i = 0; i < all; i++)
        {
            data[i] = 0.0;
            for(int j=0; j<alone && i*alone+j<sample; j++)
            {
                data[i] += samples[i*alone+j]/alone;
            }
        }

        //计算傅里叶变换
        QVector<double>out(all);
        // 创建一个FFTW计划
        fftw_complex *in_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * all));
        fftw_complex *out_ptr = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * all));

        // 将std::vector数据复制到fftw_complex数组中
        for (int i = 0; i < all; ++i) {
            in_ptr[i][0] = data[i];
            in_ptr[i][1] = 0.0;
        }

        // 创建计划并计算DFT
        fftw_plan plan = fftw_plan_dft_1d(all, in_ptr, out_ptr, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan);

        // 将结果从fftw_complex数组复制回std::vector
        for (int i = 0; i < all; ++i)
        {
            out[i] = sqrt(out_ptr[i][0]*out_ptr[i][0]+out_ptr[i][1]*out_ptr[i][1]);
        }

        // 清理
        fftw_destroy_plan(plan);
        fftw_free(in_ptr);
        fftw_free(out_ptr);
        data = out;

        //归一化
        double maxHeight = 0.0, minHeight = 0.0;
        for(int i=0; i<data.size()/2; i++)
        {
            if(data[i] > maxHeight)
            {
                maxHeight = data[i];
            }
            if(data[i] < minHeight)
            {
                minHeight = data[i];
            }
        }
        for(int i=0; i<data.size()/2; i++)
        {
            data[i] = (data[i]- minHeight)/(maxHeight - minHeight);
            data[i] = data[i]<0?0:data[i];
        }

        QList<double> sampleList = data.mid(0, all/2);
        if(m_allSamples.size() != sampleList.size()){
            m_allSamples.clear();
            m_allSamples.fill(0.0, sampleList.size());
        }

        //时间平滑函数
        const double smoothConstantDown = 0.08;
        const double smoothConstantUp = 0.8;
        for(int i=0; i<sampleList.size() && i<m_allSamples.size(); i++){
            if(std::isfinite(sampleList[i])){//判断是不是有理数
                if(sampleList[i] > m_allSamples[i]){
                    m_allSamples[i] = smoothConstantUp * sampleList[i] + (1-smoothConstantUp) * m_allSamples[i];
                }
                else{
                    m_allSamples[i] = smoothConstantDown * sampleList[i] + (1-smoothConstantDown) * m_allSamples[i];
                }
            }
        }

        int aim = 120;
        int cell = m_allSamples.size() / aim;
        QVector<double> outSamples;//处理之后的音乐样本
        outSamples.fill(0, aim);
        cell = cell <= 0 ? 1 : cell;
        for(int i=0; i<aim && i * cell < m_allSamples.size(); i++){
            double max = m_allSamples[i * cell];

            for(int j=1; j<cell && i * cell + j < m_allSamples.size(); j++){
                if(max < m_allSamples[i * cell + j]){
                    max = m_allSamples[i * cell + j];
                }
            }
            outSamples[i] = max;
        }
        emit cppDrawLine(outSamples);
    }
}
