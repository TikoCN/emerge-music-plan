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

LrcDataControl::LrcDataControl()
    : m_playingLrcId(-1) {
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
        m_startTime       += m_updateLrcTimer->interval();
        m_playingPosition = m_startTime + m_player->position();
        selectPlayLrc(m_playingPosition);
        emit lrcUpdate();
    });
}

LrcDataControl::~LrcDataControl() {
    m_updateLrcTimer->stop();
}

void LrcDataControl::loadLrcList(const int musicId) {
    m_lrcList      = FileManagement::getMusicLyricsData(musicId);
    m_playingLrcId = -1;
    emit lrcLoaded();
}

void LrcDataControl::selectPlayLrc(const qint64 time) {
    if (m_lrcList.empty()) {
        return;
    }

    // 当前歌词仍在有效范围内，无需重新搜索
    if (m_playingLrcId >= 0 && m_playingLrcId < m_lrcList.size()
        && m_lrcList[m_playingLrcId]->startTime <= time
        && m_lrcList[m_playingLrcId]->endTime   >= time) {
        return;
    }

    // 二分查找：歌词按 startTime 升序排列，O(log n) 定位目标行
    int lo = 0;
    int hi = static_cast<int>(m_lrcList.size()) - 1;
    while (lo <= hi) {
        const int mid = lo + (hi - lo) / 2;
        if (m_lrcList[mid]->endTime < time) {
            lo = mid + 1;
        } else if (m_lrcList[mid]->startTime > time) {
            hi = mid - 1;
        } else {
            // startTime <= time <= endTime，找到匹配行
            m_playingLrcId = mid;
            emit playingLrcIdChanged(mid);
            return;
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
    TLog::getInstance().logUser(tr("跳转到歌词,lrcId: %1").arg(lrcId));

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
