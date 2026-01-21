#include "basetool.h"
#include <QClipboard>
#include <QGuiApplication>

BaseTool::BaseTool() {
    typeConversion = new TypeConversion;
    fileManagement = new FileManagement;
    qmlActive = new QmlActive;
}

const QmlActive *BaseTool::getQmlActive() const
{
    return qmlActive;
}

const FileManagement *BaseTool::getFileManagement() const
{
    return fileManagement;
}

void BaseTool::copyString(QString data)
{
    QClipboard* copy = QGuiApplication::clipboard();
    copy->setText(data);
}

const TypeConversion *BaseTool::getTypeConversion() const
{
    return typeConversion;
}
