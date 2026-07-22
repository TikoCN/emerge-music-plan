#include "FileExistCheckTask.h"
#include <QFile>

FileExistCheckTask::FileExistCheckTask(const QList<QPair<int, QString> > &musicData)
    : m_musicData(musicData) {
    setAutoDelete(false);
}

void FileExistCheckTask::run() {
    QList<int> invalidIds;
    for (const auto &[fst, snd]: m_musicData) {
        if (QString url = snd; url.isEmpty() || !QFile::exists(url)) {
            invalidIds.append(fst);
        }
    }

    emit checkFinished(invalidIds);
}
