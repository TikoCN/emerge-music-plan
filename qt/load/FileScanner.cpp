#include "FileScanner.h"

#include <QDir>
#include <QUrl>

FileScanner::FileScanner(const QString &dirPath)
    : m_dirPath(dirPath) {
    setAutoDelete(false);
}

void FileScanner::scanDirectory() {
    QDir dir(m_dirPath);
    if (!dir.exists()) {
        return;
    }

    // 获取当前目录下的所有文件
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList allFiles = dir.entryInfoList();

    // 筛选音乐文件
    for (const QFileInfo &info: allFiles) {
        if (m_musicSuffix.contains(info.suffix())) {
            m_fileInfoList.append(info);
        }
    }

    // 获取子目录路径（用于后续递归扫描）
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    auto listDir = dir.entryInfoList();
    for (const QFileInfo &subDir: listDir) {
        m_subDirPaths.append(subDir.filePath());
    }
}

void FileScanner::run() {
    scanDirectory();
    emit scanFinished(m_fileInfoList, m_subDirPaths);
}
