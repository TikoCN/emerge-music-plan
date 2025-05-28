 #include "tlog.h"
#include <QDir>

TLog::TLog() {
    mm_thread = new QThread;
    this->moveToThread(mm_thread);
    mm_thread->start();

    m_file.setFileName(QDir::current().filePath("log.txt"));
    m_file.open(QIODevice::WriteOnly |QIODevice::Text);

    m_startTime = QDateTime::currentMSecsSinceEpoch();

    connect(this, &TLog::doLog, this, &TLog::log);
}

void TLog::log(TYPE type, QString str)
{
    QDateTime t = QDateTime::currentDateTime();
    QString time = t.toString("yyyy-MM-dd HH:mm:ss.zzz");
    qint64 over = t.currentMSecsSinceEpoch() - m_startTime;
    str = QString("type:%1, time: %2, over: %3, %4 \n").arg(
        m_type[(int)type],
        time,
        QString::number(over),
        str);

    if (type != DEBUG) {
        m_file.write(str.toUtf8());
        m_file.flush();
    }
    else {
        qDebug()<<str;
    }
}

void TLog::logDebug(QString str)
{
    emit doLog(DEBUG, str);
}

void TLog::logLoad(QString str)
{
    emit doLog(LOAD, str);
}

void TLog::logUser(QString str)
{
    emit doLog(USER_DO, str);
}

void TLog::logError(QString str)
{
    emit doLog(ERROR, str);
}

void TLog::logRelease(QString str)
{
    emit doLog(REALEASE, str);
}

void TLog::logInfo(QString str)
{
    emit doLog(INFO, str);
}
