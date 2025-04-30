#ifndef TLOG_H
#define TLOG_H
#include <QObject>
#include <QThread>
#include <QFile>

class TLog : public QObject
{
    Q_OBJECT
public:
    enum TYPE {DEBUG, USER_DO, LOAD, REALEASE, ERROR, INFO};
    void log(TYPE type, QString str);
    static TLog* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if (instance == nullptr){
            instance = new TLog;
        }
    }

private:
    TLog();

    static TLog* instance;
    QThread *m_t;
    QFile m_f;
};

#endif // TLOG_H
