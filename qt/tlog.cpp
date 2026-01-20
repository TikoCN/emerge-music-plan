 #include "tlog.h"
#include <QDir>

TLog::TLog() {
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    QDateTime t = QDateTime::currentDateTime();
    QString time = t.toString("yyyy-MM-dd");

    m_file.setFileName(QDir::current().filePath(time + ".txt"));
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

void TLog::log(TLog::TYPE type, QString str)
{
    QDateTime t = QDateTime::currentDateTime();
    QString time = t.toString("yyyy-MM-dd HH:mm:ss.zzz");
    qint64 over = t.currentMSecsSinceEpoch() - m_startTime;

    QString fixedType = QString("TYPE:%1").arg(m_type[(int)type].leftJustified(12));
    QString overTime = QString("OVERTIME:%1").arg(QString::number(over).leftJustified(15));

    str = QString("%1| TIME: %2| %3| %4 \n").arg(
        fixedType,
        time,
        overTime,
        str);

    m_logItemList.append(str);

    if (m_logItemList.size() > 0) {
        for (int i = 0; i < m_logItemList.length(); ++i) {
            m_file.write(m_logItemList[i].toUtf8());
        }

        m_logItemList.clear();
        m_file.flush();
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
    emit doLog(RELEASE, str);
}

void TLog::logInfo(QString str)
{
    emit doLog(INFO, str);
}

void TLog::logIgnore(QString str)
{
    emit doLog(IGNORE, str);
}
