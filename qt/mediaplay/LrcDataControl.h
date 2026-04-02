#ifndef LRCDATACONTROL_H
#define LRCDATACONTROL_H

#include "MediaplayData.h"
#include "baseclass/LrcData.h"
#include <QJsonObject>

class LrcDataControl : public MediaPlayData
{
    Q_OBJECT

protected:
    QList<LrcDataPtr> m_lrcList;//歌词
    int m_playingLrcId;
    QTimer *m_updateLrcTimer;
    long long m_startTime;
    long long m_playingPosition;

public:
    explicit LrcDataControl(BaseTool *baseTool, DataActive *dataActive, TLog *log, QObject *parent = nullptr);
    ~LrcDataControl() override;

    //加载歌词
    void loadLrcList(int musicId);

    //跳转到目标歌词
    Q_INVOKABLE void turnToLrc(int lrcId);

    Q_INVOKABLE QJsonObject getLrcJsonObject(int lrcId);
    Q_INVOKABLE LrcData getLrcData(int id);

    Q_INVOKABLE [[nodiscard]] int getLrcListLength() const;

    //选择当前播放歌词
    void selectPlayLrc(qint64 time);

    [[nodiscard]] long long getPlayingPosition() const;
    [[nodiscard]] int getPlayingLrcId() const;

    void setPlayingPosition(long newPlayingPosition);

signals:
    //下载歌词
    void downLrc(QString key, QString url, int musicId);
    //歌词加载完成
    void lrcLoaded();
    void lrcUpdate();

    void playingLrcIdChanged(int);
    void playingPositionChanged();// 播放位置更新
    void playingMusicIdChanged();

    void musicListChanged();
private:
    Q_PROPERTY(long playingPosition READ getPlayingPosition WRITE setPlayingPosition NOTIFY playingPositionChanged FINAL)
    Q_PROPERTY(int playingLrcId READ getPlayingLrcId NOTIFY playingLrcIdChanged FINAL)
};



#endif // LRCDATACONTROL_H
