#ifndef FILEEXISTCHECKTASK_H
#define FILEEXISTCHECKTASK_H

#include <QRunnable>
#include <QObject>
#include <QList>
#include <QPair>

class FileExistCheckTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    explicit FileExistCheckTask(const QList<QPair<int, QString>> &musicData);

    void run() override;

signals:
    void checkFinished(const QList<int> &invalidMusicIds);

private:
    QList<QPair<int, QString>> m_musicData;
};

#endif // FILEEXISTCHECKTASK_H
