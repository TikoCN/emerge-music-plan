#include "lrcdatacontrol.h"
#include <QJsonObject>

/*
 * 加载歌词
 */
LrcDataControl::LrcDataControl(BaseTool *baseTool, DataActive *dataActive)
    :MediaPlayData(baseTool, dataActive)
{
    connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 time){
        selectPlayLrc(time);
    });
}

void LrcDataControl::loadLrcList(int musicId)
{
    m_lrcList = m_baseTool->getFileManagement()->getMusicLyricsData(musicId);
    m_playingLrcId = -1;
    emit lrcLoaded();
}

void LrcDataControl::selectPlayLrc(qint64 time)
{
    if (m_lrcList.size() == 0){
        return;
    }

    if ((m_playingLrcId < 0 ||
         m_playingLrcId > m_lrcList.size()) ||
        (m_lrcList[m_playingLrcId]->startTime > time ||
         m_lrcList[m_playingLrcId]->endTime < time)
        ){

        // 重新筛选
        for(int i=0; i<m_lrcList.size(); i++){
            if(m_lrcList[i]->startTime <= time && m_lrcList[i]->endTime >= time){
                m_playingLrcId = i;
                emit playingLrcIdChange(i);
                break;
            }
        }
    }
}
void LrcDataControl::turnToLrc(int lrcId)
{
    if(lrcId >=0 && lrcId < m_lrcList.size()){
        m_player->setPosition(m_lrcList[lrcId]->startTime);
    }
}

QJsonObject LrcDataControl::getLrcJsonObject(int lrcId)
{
    if (lrcId < 0 || lrcId > m_lrcList.size()) {
        return QJsonObject();
    }
    return m_lrcList[lrcId]->getJsonObject();
}

int LrcDataControl::getLrcListLength()
{
    return m_lrcList.size();
}
