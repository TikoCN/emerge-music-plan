#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <QObject>
#include "baseclass/lrcdata.h"
#include <QJsonArray>

class FileManagement : public QObject
{
    Q_OBJECT
public:
    explicit FileManagement();

    // MUSIC FILE
    Q_INVOKABLE void writeMusicToFile(QStringList key, QStringList value, int musicId) const;
    Q_INVOKABLE QJsonArray getMusicAllTaglib(int musicId) const;

    // LRC FILE
    Q_INVOKABLE QString getMusicLrcUrl(int musicId) const;
    Q_INVOKABLE QString getMusicLrcData(int musicId) const;
    Q_INVOKABLE void wrtiLrcData(int musicId, QString lrcData) const;
    QList<LrcDataPtr> getMusicLyricsData(int musicId) const;

    // DIR
    Q_INVOKABLE void openPlayListDir(int playListId) const;

    QString getArtistCoverUrl(QString name) const;
    QString getAlbumCoverUrl(QString name) const;
    // 替换文件
    void replaceFile(QString inUrl, QString outUrl) const;

    QString readFileText(QString url) const;
    Q_INVOKABLE bool writeFileText(QString url, QString data) const;

    //打开文件
    Q_INVOKABLE void deskOpenFile(QString url, bool local = false) const;
    // 重命名文件
    bool renameFile(QString oldUrl, QString newUrl) const;

    Q_INVOKABLE QString getBaseUrl(QString url) const;
    Q_INVOKABLE QString getFileName(QString url) const;
    Q_INVOKABLE QString getParentDir(QString url) const;
};

#endif // FILEMANAGEMENT_H
