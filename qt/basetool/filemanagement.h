#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include "baseclass/lrcdata.h"
#include <QJsonArray>

class FileManagement : public QObject
{
    Q_OBJECT
public:
    explicit FileManagement();

    // MUSIC FILE
    Q_INVOKABLE static void writeMusicToFile(const QStringList &key, const QStringList &value, int musicId) ;
    Q_INVOKABLE [[nodiscard]] static QJsonArray getMusicAllTaglib(int musicId);

    // LRC FILE
    Q_INVOKABLE [[nodiscard]] static QString getMusicLrcUrl(int musicId);
    Q_INVOKABLE [[nodiscard]] static QString getMusicLrcData(int musicId);
    Q_INVOKABLE static void wrtiLrcData(int musicId, const QString& lrcData);
    [[nodiscard]] static QList<LrcDataPtr> getMusicLyricsData(int musicId) ;

    // DIR
    Q_INVOKABLE static void openPlayListDir(int playListId);

    [[nodiscard]] static QString getArtistCoverUrl(const QString& name);
    [[nodiscard]] static QString getAlbumCoverUrl(const QString& name) ;
    // 替换文件
    static void replaceFile(const QString& inUrl, const QString& outUrl) ;

    [[nodiscard]] static QString readFileText(const QString &url) ;
    Q_INVOKABLE [[nodiscard]] static bool writeFileText(const QString& url, const QString& data) ;

    //打开文件
    Q_INVOKABLE static void deskOpenFile(QString url, bool local = false) ;
    // 重命名文件
    [[nodiscard]] static bool renameFile(const QString& oldUrl, const QString& newUrl);

    Q_INVOKABLE [[nodiscard]] static QString getBaseUrl(const QString& url);
    Q_INVOKABLE [[nodiscard]] static QString getFileName(const QString& url) ;
    Q_INVOKABLE [[nodiscard]] static QString getParentDir(const QString& url) ;

    static void makeAllDir();
};

#endif // FILEMANAGEMENT_H
