 #include "tlog.h"
#include <QDir>

TLog::TLog() {
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    const QDateTime t = QDateTime::currentDateTime();
    m_file.setFileName(QString("data/log/%1.txt").arg(t.toString("yyyy-MM-dd")));

    if(m_file.open(QIODevice::WriteOnly |QIODevice::Text)) {

    }

    m_startTime = QDateTime::currentMSecsSinceEpoch();

    connect(this, &TLog::doLog, this, &TLog::log);
}

TLog::~TLog()
{
    m_thread->quit();
    m_file.close();
}

void TLog::log(const TLog::TYPE type, QString str, const QString& threadName, const qint64 threadId)
{
    const QDateTime t = QDateTime::currentDateTime();
    QString time = t.toString("yyyy-MM-dd HH:mm:ss.zzz");
    const qint64 over = QDateTime::currentMSecsSinceEpoch() - m_startTime;

    QString fixedType = QString("TYPE:%1").arg(m_type[static_cast<int>(type)].leftJustified(8));
    QString threadNameOut = QString("THREAD_NAME:%1").arg(threadName.leftJustified(17));
    QString threadIdOut= QString("THREAD_ID:%1").arg(QString::number(threadId).leftJustified(10));
    QString overTime = QString("OVERTIME:%1").arg(QString::number(over).leftJustified(15));

    str = QString("%1| TIME: %2  | %3| %4| %5| %6 \n").arg(
        fixedType,
        time,
        threadNameOut,
        threadIdOut,
        overTime,
        str);

    m_logItemList.append(str);

    if (!m_logItemList.empty()) {
        for (int i = 0; i < m_logItemList.length(); ++i) {
            m_file.write(m_logItemList[i].toUtf8());
        }

        m_logItemList.clear();
        m_file.flush();
    }
}

void TLog::logDebug(const QString &str)
{
    emit doLog(DEBUG, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logLoad(const QString &str)
{
    emit doLog(LOAD, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logUser(const QString &str)
{
    emit doLog(USER_DO, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logError(const QString &str)
{
    emit doLog(ERROR, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logRelease(const QString &str)
{
    emit doLog(RELEASE, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logInfo(const QString &str)
{
    emit doLog(INFO, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}

void TLog::logIgnore(const QString &str)
{
    emit doLog(IGNORE, str, QThread::currentThread()->objectName(), reinterpret_cast<qint64>(QThread::currentThreadId()));
}
