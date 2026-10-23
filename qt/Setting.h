#ifndef SETTING_H
#define SETTING_H

#include <QColor>
#include <QDir>
#include <QFont>
#include <QRectF>
#include <QJsonObject>
#include <QSettings>
#include <QTranslator>
#include "macro/SettingAttribute.h"

class QQmlApplicationEngine;

class Setting : public QObject {
    Q_OBJECT

private:
    Setting();

    const QString m_iniUrl;
    QString m_language;
    QTranslator m_translator;
    QQmlApplicationEngine *m_qmlEngine;

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
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

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

    /**
     * @brief 获取当前界面语言代码。
     * @return 当前语言代码，例如 zh_CN 或 en
     */
    QString language() const;

    /**
     * @brief 设置界面语言并刷新已创建的 QML 界面。
     * @param language 语言代码，支持 zh_CN 和 en
     */
    Q_INVOKABLE void setLanguage(const QString &language);

    /**
     * @brief 关联 QML 引擎并应用保存的界面语言。
     * @param engine 应用使用的 QML 引擎
     */
    void initializeLanguage(QQmlApplicationEngine *engine);

    //将路径移除
    Q_INVOKABLE void removeUrl(const QString &url);

    Q_INVOKABLE void writeData() const;

    //加载音乐资源
    Q_INVOKABLE void loadMusicCores();

signals:
    /**
     * @brief 界面语言发生变化时发出。
     */
    void languageChanged();

    //加载资源
    void loadMusics();
};
#endif // SETTING_H
