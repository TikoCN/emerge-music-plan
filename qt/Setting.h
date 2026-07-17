#ifndef SETTING_H
#define SETTING_H

#include <QColor>
#include <QDir>
#include <QFont>
#include <QRectF>
#include <QJsonObject>
#include <QSettings>
#include "macro/SettingAttribute.h"

class Setting : public QObject {
    Q_OBJECT

private:
    Setting();

    const QString m_iniUrl;

    QT_SETTING_PROPERTY(int, maxThreadNumber, MaxThreadNumber)

    QT_SETTING_PROPERTY(QStringList, sourceList, SourceList)

    // 颜色管理
    QT_SETTING_PROPERTY(QColor, themeColor, themeColor)
    // 歌词相关
    QT_SETTING_PROPERTY(QColor, lrcNormalColor, LrcNormalColor)
    QT_SETTING_PROPERTY(QColor, lrcPlayingColor, LrcPlayingColor)

    QT_SETTING_PROPERTY(QFont, benchmarkFont, benchmarkFont)
    QT_SETTING_PROPERTY(QFont, lrcFont, lrcFont)
    QT_SETTING_PROPERTY(QFont, deskLrcFont, DeskLrcFont)

    // 定位
    QT_SETTING_PROPERTY(QRectF, windowRect, WindowRect)
    QT_SETTING_PROPERTY(QPointF, lrcTopPoint, LrcTopPoint)

    QT_SETTING_PROPERTY(bool, isLightTheme, IsLightTheme)

    QT_SETTING_PROPERTY(bool, isOnLine, IsOnLine)
    QT_SETTING_PROPERTY(bool, isGetCoverFromNetEase, IsGetCoverFromNetEase)
    QT_SETTING_PROPERTY(bool, isGetCoverFromBing, IsGetCoverFromBing)
    QT_SETTING_PROPERTY(bool, isGetCoverFromBaidu, IsGetCoverFromBaidu)
    QT_SETTING_PROPERTY(bool, isGetCoverFromQQMusic, IsGetCoverFromQQMusic)

    QT_SETTING_PROPERTY(bool, isGetLrcFromNetEase, IsGetLrcFromNetEase)
    QT_SETTING_PROPERTY(bool, isGetLrcFromQQMusic, IsGetLrcFromQQMusic)

public:
    static Setting &getInstance() {
        static Setting instance;
        return instance;
    }

    //设置参数
    template<typename T>
    static void setParameter(const QString &key, const T &value) {
        auto *ini = new QSettings(QDir::currentPath() + "/setting.ini", QSettings::IniFormat);
        ini->beginGroup("seit");

        ini->setValue(key, value);

        ini->endGroup();
        ini->sync(); //写入磁盘
        delete ini;
    }

    //读取参数
    bool getParameterList();

    //将路径移除
    Q_INVOKABLE void removeUrl(const QString &url);

    Q_INVOKABLE void writeData() const;

    //加载音乐资源
    Q_INVOKABLE void loadMusicCores();

signals:
    //加载资源
    void loadMusics();
};
#endif // SETTING_H
