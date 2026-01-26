#include "basetool.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QThread>

BaseTool::BaseTool() {
    typeConversion = new TypeConversion;
    fileManagement = new FileManagement;
    qmlActive = new QmlActive;
}

const QmlActive *BaseTool::getQmlActive() const {
    return qmlActive;
}

const FileManagement *BaseTool::getFileManagement() const {
    return fileManagement;
}

void BaseTool::copyString(const QString &data) {
    QClipboard *copy = QGuiApplication::clipboard();
    copy->setText(data);
}

QString BaseTool::getErrorMsg(const QThread *thread, const QString &errorMsg) {
    QString result = QString("threadid: %1 threadName %2, msg: %3")
            .arg(reinterpret_cast<quint64>(QThread::currentThreadId()))
            .arg(thread->objectName())
            .arg(errorMsg);

    return result;
}

const TypeConversion *BaseTool::getTypeConversion() const {
    return typeConversion;
}
