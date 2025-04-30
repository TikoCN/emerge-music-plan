#include "tlog.h"
#include <QDir>
void TLog::log(TYPE type, QString str)
{

}

TLog::TLog() {
    m_t = new QThread;
    this->moveToThread(m_t);
    m_t->start();

    m_f.setFileName(QDir::current().filePath("log.text"));
    m_f.open(QIODevice::ReadWrite |QIODevice::Text);
}
