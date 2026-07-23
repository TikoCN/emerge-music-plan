#ifndef DATALOADER_H
#define DATALOADER_H

#include <atomic>
#include <QObject>
#include <QString>

class DataLoader : public QObject {
    Q_OBJECT

private:
    std::atomic<int> m_loadPos{0};
    std::atomic<bool> m_loading{false};
    std::atomic<bool> m_loadInit{false};
    std::atomic<bool> m_loadIsFinish{false};
    std::atomic<bool> m_loadEnable{true};
    int m_loadSize{20};
    QString m_currentKey{""};

public:
    explicit DataLoader(QObject *parent = nullptr);

    Q_INVOKABLE void setLoadSize(int size);
    Q_INVOKABLE int getLoadSize() const;

    Q_INVOKABLE void setCurrentKey(const QString &key);
    Q_INVOKABLE QString getCurrentKey() const;

    Q_INVOKABLE bool isLoading() const;
    Q_INVOKABLE bool isLoadInit() const;
    Q_INVOKABLE bool isLoadFinish() const;
    Q_INVOKABLE bool isLoadEnable() const;

    Q_INVOKABLE void loadMore();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void init();

    Q_INVOKABLE void setLoadFinish(bool finish);
    Q_INVOKABLE void setLoadEnable(bool enable);
    Q_INVOKABLE void finishLoading();

signals:
    void loadData(int index, const QString &key, QObject *callback);
    void loadInitData(const QString &key, QObject *callback);
};

#endif // DATALOADER_H