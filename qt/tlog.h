#ifndef TLOG_H
#define TLOG_H
#include <QObject>
#include <QThread>
#include <QFile>
#include <QTextStream>

class TLog : public QObject
{
    Q_OBJECT
public:
    enum TYPE {DEBUG, USER_DO, LOAD, RELEASE, ERROR, INFO, IGNORE};
    void log(TYPE type, QString str);
    static TLog* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if (instance == nullptr){
            instance = new TLog;
        }
    }

    void logDebug(QString str);
    void logLoad(QString str);
    void logUser(QString str);
    void logError(QString str);
    void logRelease(QString str);
    void logInfo(QString str);
    void logIgnore(QString str);

private:
    TLog();
    ~TLog();

    static TLog* instance;
    QThread *m_thread;
    QFile m_file;
    qint64 m_startTime;
    QStringList m_logItemList;
    const QStringList m_type = {
        "DEBUG",
        "USER_DO",
        "LOAD",
        "RELEASE",
        "ERROR",
        "INFO",
        "IGNORE"
    };

signals:
    void doLog(TLog::TYPE type, QString str);
};

#endif // TLOG_H
