#ifndef TASKBARMANAGER_H
#define TASKBARMANAGER_H

#include <QObject>
#include <QPointer>

class QSystemTrayIcon;
class QMenu;
class QAction;
class QWindow;
class MediaPlayer;

/**
 * @brief 管理系统托盘以及主界面和桌面歌词窗口。
 */
class TaskbarManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 创建托盘图标、系统风格菜单并初始化窗口管理器。
     * @param parent 父对象
     */
    explicit TaskbarManager(QObject *parent = nullptr);

    /**
     * @brief 释放托盘菜单。
     */
    ~TaskbarManager() override;

    /**
     * @brief 注册主界面窗口。
     * @param window QML 主界面对象
     */
    Q_INVOKABLE void setMainWindow(QObject *window);

    /**
     * @brief 注册或清除桌面歌词窗口。
     * @param window QML 桌面歌词对象；传入空对象时清除注册
     */
    Q_INVOKABLE void setLyricsWindow(QObject *window);

    /**
     * @brief 显示并激活主界面。
     */
    Q_INVOKABLE void showMainWindow();

    /**
     * @brief 隐藏主界面到系统托盘。
     */
    Q_INVOKABLE void hideMainWindow();

    /**
     * @brief 按主界面当前状态切换显示或隐藏。
     */
    Q_INVOKABLE void toggleMainWindow();

    /**
     * @brief 显示桌面歌词窗口。
     */
    Q_INVOKABLE void showLyricsWindow();

    /**
     * @brief 隐藏桌面歌词窗口。
     */
    Q_INVOKABLE void hideLyricsWindow();

    /**
     * @brief 按歌词窗口当前状态切换显示或隐藏。
     */
    Q_INVOKABLE void toggleLyricsWindow();

signals:
    /**
     * @brief 请求 QML 首次创建桌面歌词窗口。
     */
    void lyricsWindowRequested();

private:
    /**
     * @brief 刷新托盘中的歌曲信息和播放控制文字。
     */
    void updatePlaybackActions();

    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    QAction *m_currentTrackAction;
    QAction *m_previousAction;
    QAction *m_playPauseAction;
    QAction *m_nextAction;
    QAction *m_mainAction;
    QAction *m_lyricsAction;
    QAction *m_quitAction;
    QPointer<QWindow> m_mainWindow;
    QPointer<QWindow> m_lyricsWindow;
    MediaPlayer *m_mediaPlayer;
};

#endif // TASKBARMANAGER_H
