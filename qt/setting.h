#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QRectF>
#include <QJsonObject>

class Setting :public QObject
{
    Q_OBJECT
private:
    static Setting* instance;
    Setting();

    Q_PROPERTY(int maxThreadNumber READ getMaxThreadNumber WRITE setMaxThreadNumber NOTIFY maxThreadNumberChanged FINAL)

    Q_PROPERTY(QStringList sourceList READ getSourceList WRITE setSourceList NOTIFY sourceListChanged FINAL)

    Q_PROPERTY(QColor themeColor READ getThemeColor WRITE setThemeColor NOTIFY themeColorChanged FINAL)

    Q_PROPERTY(QColor transparentColor READ getTransparentColor WRITE setTransparentColor NOTIFY transparentColorChanged FINAL)

    Q_PROPERTY(QColor backdropColor READ getBackdropColor WRITE setBackdropColor NOTIFY backdropColorChanged FINAL)

    Q_PROPERTY(QFont mainFont READ getMainFont WRITE setMainFont NOTIFY mainFontChanged FINAL)

    Q_PROPERTY(QFont deskFont READ getDeskFont WRITE setDeskFont NOTIFY deskFontChanged FINAL)

    Q_PROPERTY(QRectF windowRect READ getWindowRect WRITE setWindowRect NOTIFY windowRectChanged FINAL)

    Q_PROPERTY(QFont mainLrcFont READ getMainLrcFont WRITE setMainLrcFont NOTIFY mainLrcFontChanged FINAL)

    Q_PROPERTY(QPoint lrcTopPoint READ getLrcTopPoint WRITE setLrcTopPoint NOTIFY lrcTopPointChanged FINAL)

    Q_PROPERTY(bool isOnLine READ getIsOnLine WRITE setIsOnLine NOTIFY isOnLineChanged FINAL)

    Q_PROPERTY(bool isGetCoverFromNetEase READ getIsGetCoverFromNetEase WRITE setIsGetCoverFromNetEase NOTIFY isGetCoverFromNetEaseChanged FINAL)

    Q_PROPERTY(bool isGetCoverFromBing READ getIsGetCoverFromBing WRITE setIsGetCoverFromBing NOTIFY isGetCoverFromBingChanged FINAL)

    Q_PROPERTY(bool isGetCoverFromBaidu READ getIsGetCoverFromBaidu WRITE setIsGetCoverFromBaidu NOTIFY isGetCoverFromBaiduChanged FINAL)

    Q_PROPERTY(bool isGetLrcFromNetEase READ getIsGetLrcFromNetEase WRITE setIsGetLrcFromNetEase NOTIFY isGetLrcFromNetEaseChanged FINAL)

    Q_PROPERTY(bool isGetCoverFromQQMusic READ getIsGetCoverFromQQMusic WRITE setIsGetCoverFromQQMusic NOTIFY isGetCoverFromQQMusicChanged FINAL)

    Q_PROPERTY(bool isGetLrcFromQQMusic READ getIsGetLrcFromQQMusic WRITE setIsGetLrcFromQQMusic NOTIFY isGetLrcFromQQMusicChanged FINAL)

    Q_PROPERTY(QColor deskLrcColor READ getDeskLrcColor WRITE setDeskLrcColor NOTIFY deskLrcColorChanged FINAL)

public:
    static Setting* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new Setting;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    bool isOnLine;
    bool isGetCoverFromNetEase;
    bool isGetCoverFromQQMusic;
    bool isGetCoverFromBing;
    bool isGetCoverFromBaidu;
    bool isGetLrcFromNetEase;
    bool isGetLrcFromQQMusic;

    int maxThreadNumber;//最大线程数量

    QStringList sourceList;//资源地址列表

    QColor themeColor;//主题颜色
    QColor transparentColor;//透明层颜色
    QColor backdropColor;//背景颜色
    QColor deskLrcColor; //桌面歌词颜色

    QPoint lrcTopPoint;//桌面歌词位置
    QRectF windowRect;//主界面

    QFont deskFont;//桌面字体
    QFont mainLrcFont;//主页歌词颜色
    QFont mainFont;//主界面字体

    //设置参数
    template <typename T>
    void setParameter(QString key, T value);

    //读取参数
    bool getParameterList();

    //将路径移除
    Q_INVOKABLE void removeUrl(QString url);

    Q_INVOKABLE void writeData();

    //加载音乐资源
    Q_INVOKABLE void loadMusicCores();

    QStringList getSourceList() const;
    void setSourceList(const QStringList &newSourceList);

    int getMaxThreadNumber() const;
    void setMaxThreadNumber(int newMaxThreadNumber);

    QColor getThemeColor() const;
    void setThemeColor(const QColor &newthemeColor);

    QColor getTransparentColor() const;
    void setTransparentColor(const QColor &newtransparentColor);

    QColor getBackdropColor() const;
    void setBackdropColor(const QColor &newbackdropColor);

    QFont getMainFont() const;
    void setMainFont(const QFont &newmainFont);

    QFont getDeskFont() const;
    void setDeskFont(const QFont &newdeskFont);

    QRectF getWindowRect() const;
    void setWindowRect(const QRectF &newWindowRect);

    QFont getMainLrcFont() const;
    void setMainLrcFont(const QFont &newMainLrcFont);

    QPoint getLrcTopPoint() const;
    void setLrcTopPoint(QPoint newLrcTopPoint);

    bool getIsOnLine() const;
    void setIsOnLine(bool newIsOnLine);

    bool getIsGetCoverFromNetEase() const;
    void setIsGetCoverFromNetEase(bool newIsGetCoverFromNetEase);

    bool getIsGetCoverFromBing() const;
    void setIsGetCoverFromBing(bool newIsGetCoverFromBing);

    bool getIsGetCoverFromBaidu() const;
    void setIsGetCoverFromBaidu(bool newIsGetCoverFromBaidu);

    bool getIsGetLrcFromNetEase() const;
    void setIsGetLrcFromNetEase(bool newIsGetLrcFromNetEase);

    bool getIsGetCoverFromQQMusic() const;
    void setIsGetCoverFromQQMusic(bool newIsGetCoverFromQQMusic);

    bool getIsGetLrcFromQQMusic() const;
    void setIsGetLrcFromQQMusic(bool newIsGetLrcFromQQMusic);

    QColor getDeskLrcColor() const;
    void setDeskLrcColor(const QColor &newDeskLrcColor);

signals:
    //加载资源
    void loadMusics();

    void maxThreadNumberChanged();
    void sourceListChanged();
    void themeColorChanged();
    void transparentColorChanged();
    void backdropColorChanged();
    void mainFontChanged();
    void deskFontChanged();
    void windowRectChanged();
    void mainLrcFontChanged();
    void lrcTopPointChanged();
    void isOnLineChanged();
    void isGetCoverFromNetEaseChanged();
    void isGetCoverFromBingChanged();
    void isGetCoverFromBaiduChanged();
    void isGetLrcFromNetEaseChanged();
    void isGetCoverFromQQMusicChanged();
    void isGetLrcFromQQMusicChanged();
    void deskLrcColorChanged();
};
#endif // SETTING_H
