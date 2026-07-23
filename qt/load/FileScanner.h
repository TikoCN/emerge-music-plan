#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <QObject>
#include <QRunnable>
#include <QFileInfoList>
#include <QSet>
#include <QString>

class FileScanner : public QObject, public QRunnable {
    Q_OBJECT

private:
    QString m_dirPath;
    QFileInfoList m_fileInfoList;
    QStringList m_subDirPaths;

    const QSet<QString> m_musicSuffix = {"mp3", "flac", "m4a", "aav", "wma", "pcm"};

    void scanDirectory();

public:
    explicit FileScanner(const QString &dirPath);

    void run() override;

signals:
    void scanFinished(QFileInfoList fileInfoList, QStringList subDirPaths);
};

#endif // FILESCANNER_H