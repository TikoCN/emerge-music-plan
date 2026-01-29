#ifndef SETTING_H
#define SETTING_H

#include <QColor>
#include <QDir>
#include <QFont>
#include <QRectF>
#include <QJsonObject>
#include <QSettings>

#define QT_SETTING_PROPERTY(TYPE, PROP, PROP_CAMEL) \
/* 1. 生成Q_PROPERTY声明 */ \
Q_PROPERTY(TYPE PROP READ get##PROP_CAMEL WRITE set##PROP_CAMEL NOTIFY PROP##Changed FINAL) \
/* 2. 私有成员变量（命名规范：m_ + 小写属性名） */ \
private: \
TYPE m_##PROP; \
const QString m_##PROP##Key = #PROP; \
static const bool m_##PROP##_registered; \
/* 3. 公共GETTER方法 */ \
public: \
TYPE get##PROP_CAMEL() const { return m_##PROP; } \
/* 4. 带校验的SETTER方法（信号+INI持久化） */ \
void set##PROP_CAMEL(const TYPE &new##PROP_CAMEL) { \
if (m_##PROP == new##PROP_CAMEL) return; /* 值未变化则直接返回 */ \
m_##PROP = new##PROP_CAMEL; \
Setting::setParameter(m_##PROP##Key, m_##PROP); /* 基础类型直接写入 */ \
emit PROP##Changed();\
}

class Setting :public QObject
{
    Q_OBJECT
private:
    static Setting* instance;
    Setting();

    const QString m_iniUrl;

    QT_SETTING_PROPERTY(int, maxThreadNumber, MaxThreadNumber);

    QT_SETTING_PROPERTY(QStringList, sourceList, SourceList);

    QT_SETTING_PROPERTY(QColor, themeColor, ThemeColor);
    QT_SETTING_PROPERTY(QColor, transparentColor, TransparentColor);
    QT_SETTING_PROPERTY(QColor, backdropColor, BackdropColor);
    QT_SETTING_PROPERTY(QColor, deskLrcColor, DeskLrcColor);

    QT_SETTING_PROPERTY(QFont, mainFont, MainFont);
    QT_SETTING_PROPERTY(QFont, deskFont, DeskFont);
    QT_SETTING_PROPERTY(QFont, mainLrcFont, MainLrcFont);

    QT_SETTING_PROPERTY(QRectF, windowRect, WindowRect);

    QT_SETTING_PROPERTY(QPointF, lrcTopPoint, LrcTopPoint);

    QT_SETTING_PROPERTY(bool, isOnLine, IsOnLine);
    QT_SETTING_PROPERTY(bool, isGetCoverFromNetEase, IsGetCoverFromNetEase);
    QT_SETTING_PROPERTY(bool, isGetCoverFromBing, IsGetCoverFromBing);
    QT_SETTING_PROPERTY(bool, isGetCoverFromBaidu, IsGetCoverFromBaidu);
    QT_SETTING_PROPERTY(bool, isGetCoverFromQQMusic, IsGetCoverFromQQMusic);

    QT_SETTING_PROPERTY(bool, isGetLrcFromNetEase, IsGetLrcFromNetEase);
    QT_SETTING_PROPERTY(bool, isGetLrcFromQQMusic, IsGetLrcFromQQMusic);

public:
    static Setting* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new Setting;
        }
    }

    static void freeInstance(){\
            delete instance;
    }
    //设置参数
    template <typename T>
    static void setParameter(const QString& key, const T& value) {
        auto *ini = new QSettings(QDir::currentPath() + "/setting.ini", QSettings::IniFormat);
        ini->beginGroup("seit");

        ini->setValue(key, value);

        ini->endGroup();
        ini->sync();//写入磁盘
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

    void isOnLineChanged();
    void isGetCoverFromNetEaseChanged();
    void isGetCoverFromQQMusicChanged();
    void isGetCoverFromBingChanged();
    void isGetCoverFromBaiduChanged();
    void isGetLrcFromNetEaseChanged();
    void isGetLrcFromQQMusicChanged();
    void maxThreadNumberChanged();
    void sourceListChanged();
    void themeColorChanged();
    void transparentColorChanged();
    void backdropColorChanged();
    void deskLrcColorChanged();
    void mainFontChanged();
    void deskFontChanged();
    void mainLrcFontChanged();
    void lrcTopPointChanged();
    void windowRectChanged();
};
#endif // SETTING_H
