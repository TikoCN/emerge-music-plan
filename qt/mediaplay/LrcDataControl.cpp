#include "LrcDataControl.h"
#include <QJsonObject>
#include <QTimer>

/*
 * 加载歌词
 */
void LrcDataControl::setPlayingPosition(long newPlayingPosition) {
    if (m_playingPosition == newPlayingPosition)
        return;
    m_playingPosition = newPlayingPosition;
    emit playingPositionChanged();
}

LrcDataControl::LrcDataControl(BaseTool *baseTool, DataActive *dataActive, TLog *log, QObject *parent)
    : MediaPlayData(baseTool, dataActive, log, parent)
      , m_playingLrcId(-1) {
    connect(m_player, &QMediaPlayer::playingChanged, this, [this](const bool isPlaying) {
        if (isPlaying) {
            m_updateLrcTimer->start(20);
        } else {
            m_updateLrcTimer->stop();
        }
    });

    connect(m_player, &QMediaPlayer::positionChanged, this, [this](const qint64 time) {
        m_startTime = 0;
    });

    m_updateLrcTimer = new QTimer(this);
    connect(m_updateLrcTimer, &QTimer::timeout, this, [this]() {
        m_startTime += m_updateLrcTimer->interval();
        m_playingPosition = m_startTime + m_player->position();
        selectPlayLrc(m_playingPosition);
        emit lrcUpdate();
    });
}

LrcDataControl::~LrcDataControl() {
    delete m_updateLrcTimer;
}

void LrcDataControl::loadLrcList(const int musicId) {
    m_lrcList = FileManagement::getMusicLyricsData(musicId);
    m_playingLrcId = -1;
    emit lrcLoaded();
}

void LrcDataControl::selectPlayLrc(const qint64 time) {
    if (m_lrcList.empty()) {
        return;
    }

    if ((m_playingLrcId < 0 ||
         m_playingLrcId > m_lrcList.size()) ||
        (m_lrcList[m_playingLrcId]->startTime > time ||
         m_lrcList[m_playingLrcId]->endTime < time)
    ) {
        // 重新筛选
        for (int i = 0; i < m_lrcList.size(); i++) {
            if (m_lrcList[i]->startTime <= time && m_lrcList[i]->endTime >= time) {
                m_playingLrcId = i;
                emit playingLrcIdChanged(i);
                break;
            }
        }
    }
}

long long LrcDataControl::getPlayingPosition() const {
    return m_playingPosition;
}

int LrcDataControl::getPlayingLrcId() const {
    return m_playingLrcId;
}

void LrcDataControl::turnToLrc(const int lrcId) {
    m_loger->logUser(tr("跳转到歌词,lrcId: %1").arg(lrcId));

    if (lrcId >= 0 && lrcId < m_lrcList.size()) {
        m_player->setPosition(m_lrcList[lrcId]->startTime);
    }
}

QJsonObject LrcDataControl::getLrcJsonObject(const int lrcId) {
    if (lrcId < 0 || lrcId > m_lrcList.size()) {
        return {};
    }
    return m_lrcList[lrcId]->getJsonObject();
}

LrcData LrcDataControl::getLrcData(const int id) {
    if (0 <= id && id < m_lrcList.size()) {
        return *m_lrcList[id];
    }

    return {};
}

int LrcDataControl::getLrcListLength() const {
    return static_cast<int>(m_lrcList.size());
}
