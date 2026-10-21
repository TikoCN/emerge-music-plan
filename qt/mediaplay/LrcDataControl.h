#ifndef LRCDATACONTROL_H
#define LRCDATACONTROL_H

#include "MediaplayData.h"
#include "baseclass/LrcData.h"
#include "model/LrcModel.h"
#include <QJsonObject>

class LrcDataControl : public MediaPlayData {
    Q_OBJECT

protected:
    QList<LrcDataPtr> m_lrcList; //歌词
    LrcModel *        m_lrcModel;
    int               m_playingLrcId;
    QTimer *          m_updateLrcTimer;
    qint64            m_playingPosition;

public:
    explicit LrcDataControl();
    ~LrcDataControl() override;

    //加载歌词
    void loadLrcList(int musicId);

    //跳转到目标歌词
    Q_INVOKABLE void turnToLrc(int lrcId);

    Q_INVOKABLE QJsonObject getLrcJsonObject(int lrcId);
    Q_INVOKABLE LrcData     getLrcData(int id);

    Q_INVOKABLE [[nodiscard]] int getLrcListLength() const;
    /**
     * @brief 获取供 QML 歌词列表使用的模型。
     * @return 歌词列表模型。
     */
    [[nodiscard]] LrcModel *getLrcModel() const;
    /**
     * @brief 从已加载歌词恢复模型内容。
     */
    Q_INVOKABLE void refreshLrcModel();
    /**
     * @brief 清空歌词列表模型。
     */
    Q_INVOKABLE void clearLrcModel();

    //选择当前播放歌词
    void selectPlayLrc(qint64 time);

    /**
     * @brief 获取与播放器同步的歌词播放位置。
     * @return 播放位置，单位为毫秒。
     */
    [[nodiscard]] qint64 getPlayingPosition() const;
    [[nodiscard]] int       getPlayingLrcId() const;

    /**
     * @brief 更新歌词播放位置并通知界面。
     * @param newPlayingPosition 播放位置，单位为毫秒。
     */
    void setPlayingPosition(qint64 newPlayingPosition);

signals:
    //下载歌词
    void downLrc(QString key, QString url, int musicId);
    //歌词加载完成
    void lrcLoaded();
    void lrcUpdate();

    void playingLrcIdChanged(int);
    void playingPositionChanged(); // 播放位置更新
    void playingMusicIdChanged();

    void musicListChanged();

private:
    Q_PROPERTY(qint64 playingPosition READ getPlayingPosition WRITE setPlayingPosition NOTIFY playingPositionChanged FINAL)
    Q_PROPERTY(int playingLrcId READ getPlayingLrcId NOTIFY playingLrcIdChanged FINAL)
    Q_PROPERTY(LrcModel *lrcModel READ getLrcModel CONSTANT FINAL)
};


#endif // LRCDATACONTROL_H
