#ifndef LRCDATACONTROL_H
#define LRCDATACONTROL_H

#include "mediaplaydata.h"
#include "baseclass/lrcdata.h"
#include <QJsonObject>

class LrcDataControl : public MediaPlayData
{
    Q_OBJECT

protected:
    QList<LrcDataPtr> m_lrcList;//歌词
    int m_playingLrcId;

public:
    explicit LrcDataControl(BaseTool *baseTool, DataActive *dataActive);

    //加载歌词
    void loadLrcList(int musicId);

    //跳转到目标歌词
    Q_INVOKABLE void turnToLrc(int lrcId);

    Q_INVOKABLE QJsonObject getLrcJsonObject(int lrcId);

    Q_INVOKABLE [[nodiscard]] int getLrcListLength() const;

    //选择当前播放歌词
    void selectPlayLrc(qint64 time);

signals:
    //下载歌词
    void downLrc(QString key, QString url, int musicId);
    //歌词加载完成
    void lrcLoaded();
    void playingLrcIdChange(int);
    void playingMusicIdChanged();
    void musicListChanged();
};

#endif // LRCDATACONTROL_H
