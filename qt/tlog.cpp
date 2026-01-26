 #include "tlog.h"
#include <QDir>

TLog::TLog() {
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    const QDateTime t = QDateTime::currentDateTime();
    if (!QDir().mkpath("log")) { // 自动创建log目录（不存在时）
        qDebug()<<"創建日志文件夹失敗";
    }
    m_file.setFileName(QString("log/%1.txt").arg(t.toString("yyyy-MM-dd")));

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

void TLog::log(const TLog::TYPE type, QString str)
{
    const QDateTime t = QDateTime::currentDateTime();
    QString time = t.toString("yyyy-MM-dd HH:mm:ss.zzz");
    const qint64 over = QDateTime::currentMSecsSinceEpoch() - m_startTime;

    QString fixedType = QString("TYPE:%1").arg(m_type[static_cast<int>(type)].leftJustified(12));
    QString overTime = QString("OVERTIME:%1").arg(QString::number(over).leftJustified(15));

    str = QString("%1| TIME: %2| %3| %4 \n").arg(
        fixedType,
        time,
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
    emit doLog(DEBUG, str);
}

void TLog::logLoad(const QString &str)
{
    emit doLog(LOAD, str);
}

void TLog::logUser(const QString &str)
{
    emit doLog(USER_DO, str);
}

void TLog::logError(const QString &str)
{
    emit doLog(ERROR, str);
}

void TLog::logRelease(const QString &str)
{
    emit doLog(RELEASE, str);
}

void TLog::logInfo(const QString &str)
{
    emit doLog(INFO, str);
}

void TLog::logIgnore(const QString &str)
{
    emit doLog(IGNORE, str);
}
