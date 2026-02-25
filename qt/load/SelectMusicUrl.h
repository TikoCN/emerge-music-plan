#ifndef SELECTMUSICURL_H
#define SELECTMUSICURL_H

#include <QRunnable>
#include <QFileInfoList>

class SelectMusicUrl : public QObject, public QRunnable
{
    Q_OBJECT
private:
    QFileInfoList m_list;

    const QSet<QString> m_musicSuffix = {"mp3", "flac", "m4a", "aav", "wma", "pcm"};

public:
    explicit SelectMusicUrl(QFileInfoList list);

    void run() override;

signals:
    void fileSelected(QFileInfoList);
};

#endif // SELECTMUSICURL_H
