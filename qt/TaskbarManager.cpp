#include "TaskbarManager.h"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QCursor>
#include <QImage>
#include <QIcon>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QSize>
#include <QSystemTrayIcon>
#include <QWindow>

#include "datacore/DataActive.h"
#include "mediaplay/Mediaplayer.h"

namespace {

/**
 * @brief 按系统菜单文字颜色生成单色图标。
 * @param resource 图标资源路径
 * @return 使用系统前景色着色的图标
 */
QIcon createMenuIcon(const QString &resource,
                     const QColor &iconColor = QApplication::palette().color(QPalette::WindowText)) {
    QPixmap pixmap = QIcon(resource).pixmap(QSize(32, 32));
    if (pixmap.isNull()) {
        return QIcon(resource);
    }

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(image.rect(), iconColor);
    return QIcon(QPixmap::fromImage(image));
}

}

/**
 * @brief 创建系统托盘图标和原生菜单。
 * @param parent 父对象
 */
TaskbarManager::TaskbarManager(QObject *parent)
    : QObject(parent),
      m_trayIcon(new QSystemTrayIcon(createMenuIcon(QStringLiteral(":/image/music.svg")), this)),
      m_trayMenu(new QMenu),
      m_currentTrackAction(nullptr),
      m_previousAction(nullptr),
      m_playPauseAction(nullptr),
      m_nextAction(nullptr),
      m_mainAction(nullptr),
      m_lyricsAction(nullptr),
      m_quitAction(nullptr),
      m_mainWindow(nullptr),
      m_lyricsWindow(nullptr),
      m_mediaPlayer(&MediaPlayer::getInstance()) {
    m_currentTrackAction = m_trayMenu->addAction(QString());
    m_currentTrackAction->setEnabled(false);
    m_previousAction = m_trayMenu->addAction(createMenuIcon(QStringLiteral(":/image/up.svg")), tr("上一首"));
    m_playPauseAction = m_trayMenu->addAction(QString());
    m_nextAction = m_trayMenu->addAction(createMenuIcon(QStringLiteral(":/image/down.svg")), tr("下一首"));
    m_trayMenu->addSeparator();
    m_mainAction = m_trayMenu->addAction(QIcon(), tr("主界面"));
    m_mainAction->setCheckable(true);
    m_lyricsAction = m_trayMenu->addAction(QIcon(), tr("歌词"));
    m_lyricsAction->setCheckable(true);
    m_trayMenu->addSeparator();
    m_quitAction = m_trayMenu->addAction(createMenuIcon(QStringLiteral(":/image/close.svg")), tr("退出程序"));

    connect(m_previousAction, &QAction::triggered, m_mediaPlayer, [this]() {
        m_mediaPlayer->playNext(-1);
    });
    connect(m_playPauseAction, &QAction::triggered, this, [this]() {
        if (m_mediaPlayer->getPlayer()->playbackState() == QMediaPlayer::PlayingState) {
            m_mediaPlayer->getPlayer()->pause();
        } else {
            m_mediaPlayer->getPlayer()->play();
        }
    });
    connect(m_nextAction, &QAction::triggered, m_mediaPlayer, [this]() {
        m_mediaPlayer->playNext(1);
    });
    connect(m_mainAction, &QAction::triggered, this, &TaskbarManager::toggleMainWindow);
    connect(m_lyricsAction, &QAction::triggered, this, &TaskbarManager::toggleLyricsWindow);
    connect(m_quitAction, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
    connect(m_trayMenu, &QMenu::aboutToShow, this, [this]() {
        m_previousAction->setIcon(createMenuIcon(QStringLiteral(":/image/up.svg")));
        m_nextAction->setIcon(createMenuIcon(QStringLiteral(":/image/down.svg")));
        updateWindowActions();
        m_quitAction->setIcon(createMenuIcon(QStringLiteral(":/image/close.svg")));
        updatePlaybackActions();
    });

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, [this](const QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
            m_trayMenu->popup(QCursor::pos());
        }
    });
    connect(m_mediaPlayer, &MediaPlayer::playingMusicIdChanged, this, &TaskbarManager::updatePlaybackActions);
    connect(m_mediaPlayer->getPlayer(), &QMediaPlayer::playbackStateChanged, this,
            [this](const QMediaPlayer::PlaybackState) {
                updatePlaybackActions();
            });
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->show();
    updatePlaybackActions();
}

/**
 * @brief 释放托盘菜单。
 */
TaskbarManager::~TaskbarManager() {
    m_trayIcon->setContextMenu(nullptr);
    delete m_trayMenu;
}

void TaskbarManager::retranslateUi() {
    m_previousAction->setText(tr("上一首"));
    m_nextAction->setText(tr("下一首"));
    m_mainAction->setText(tr("主界面"));
    m_lyricsAction->setText(tr("歌词"));
    m_quitAction->setText(tr("退出程序"));
    updatePlaybackActions();
}

/**
 * @brief 注册主界面窗口。
 * @param window QML 主界面对象
 */
void TaskbarManager::setMainWindow(QObject *window) {
    m_mainWindow = qobject_cast<QWindow *>(window);
    if (m_mainWindow != nullptr) {
        connect(m_mainWindow, &QWindow::visibleChanged, this, [this]() {
            updateWindowActions();
        });
    }
    updateWindowActions();
}

/**
 * @brief 注册或清除桌面歌词窗口。
 * @param window QML 桌面歌词对象；传入空对象时清除注册
 */
void TaskbarManager::setLyricsWindow(QObject *window) {
    m_lyricsWindow = qobject_cast<QWindow *>(window);
    if (m_lyricsWindow != nullptr) {
        connect(m_lyricsWindow, &QWindow::visibleChanged, this, [this]() {
            updateWindowActions();
        });
    }
    updateWindowActions();
}

/**
 * @brief 显示并激活主界面。
 */
void TaskbarManager::showMainWindow() {
    if (m_mainWindow == nullptr) {
        return;
    }

    m_mainWindow->show();
    m_mainWindow->raise();
    m_mainWindow->requestActivate();
}

/**
 * @brief 隐藏主界面到系统托盘。
 */
void TaskbarManager::hideMainWindow() {
    if (m_mainWindow != nullptr) {
        m_mainWindow->hide();
    }
}

/**
 * @brief 按主界面当前状态切换显示或隐藏。
 */
void TaskbarManager::toggleMainWindow() {
    if (m_mainWindow != nullptr && m_mainWindow->isVisible()) {
        hideMainWindow();
        return;
    }

    showMainWindow();
}

/**
 * @brief 显示桌面歌词窗口。
 */
void TaskbarManager::showLyricsWindow() {
    if (m_lyricsWindow == nullptr) {
        emit lyricsWindowRequested();
        return;
    }

    m_lyricsWindow->show();
    m_lyricsWindow->raise();
    m_lyricsWindow->requestActivate();
}

/**
 * @brief 隐藏桌面歌词窗口。
 */
void TaskbarManager::hideLyricsWindow() {
    if (m_lyricsWindow != nullptr) {
        m_lyricsWindow->hide();
    }
}

/**
 * @brief 按歌词窗口当前状态切换显示或隐藏。
 */
void TaskbarManager::toggleLyricsWindow() {
    if (m_lyricsWindow != nullptr && m_lyricsWindow->isVisible()) {
        hideLyricsWindow();
        return;
    }

    showLyricsWindow();
}

/**
 * @brief 按窗口显示状态更新托盘菜单项及图标颜色。
 */
void TaskbarManager::updateWindowActions() {
    const QColor regularColor = QApplication::palette().color(QPalette::WindowText);
    const QColor themeColor = QApplication::palette().color(QPalette::Highlight);
    const bool mainVisible = m_mainWindow != nullptr && m_mainWindow->isVisible();
    const bool lyricsVisible = m_lyricsWindow != nullptr && m_lyricsWindow->isVisible();
    m_mainAction->setChecked(mainVisible);
    m_lyricsAction->setChecked(lyricsVisible);
    m_mainAction->setIcon(createMenuIcon(QStringLiteral(":/image/music.svg"),
                                         mainVisible ? themeColor : regularColor));
    m_lyricsAction->setIcon(createMenuIcon(QStringLiteral(":/image/lrc.svg"),
                                           lyricsVisible ? themeColor : regularColor));
}

/**
 * @brief 刷新托盘菜单中的歌曲信息和播放控制文字。
 */
void TaskbarManager::updatePlaybackActions() {
    MusicPtr music;
    if (m_mediaPlayer->playingMusicId() >= 0) {
        music = DataActive::getInstance().getMusicCore(m_mediaPlayer->playingMusicId());
    }

    if (music.isNull()) {
        m_currentTrackAction->setText(tr("正在播放：暂无歌曲"));
        m_trayIcon->setToolTip(tr("尘星音乐"));
    } else {
        const QString trackText = tr("正在播放：%1 - %2").arg(music->title, music->artist);
        m_currentTrackAction->setText(trackText);
        m_trayIcon->setToolTip(trackText);
    }

    const bool isPlaying = m_mediaPlayer->getPlayer()->playbackState() == QMediaPlayer::PlayingState;
    m_playPauseAction->setText(isPlaying ? tr("暂停") : tr("播放"));
    m_playPauseAction->setIcon(createMenuIcon(isPlaying ? QStringLiteral(":/image/stop.svg")
                                                         : QStringLiteral(":/image/play.svg")));
}
