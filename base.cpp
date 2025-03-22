#include "base.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QGuiApplication>

void Base::replaceFile(QString inUrl, QString outUrl)
{
    bool r = false;
    QFile file(inUrl);
    if(file.exists()){
        r = file.remove();
        if(!r){
            sendMessage(inUrl + tr("无法删除"), 1);
        }
        return;
    }
    file.close();

    QFile out(outUrl);
    if(out.exists()){
        r = out.rename(inUrl);
        if(!r){
            sendMessage(outUrl + tr("重命名"), 1);
        }
    }
    out.close();
}

QString Base::readFileText(QString url)
{
    QString data = "";
    QFile file(url);
    if(!file.exists()){
        emit sendMessage(url + tr(" 文件不存在"),1);
        return data;
    }

    if(!file.open(QIODevice::Text |QIODevice::ReadOnly)){
        emit sendMessage(url + tr(" 打开文件失败"),1);
        return data;
    }

    QTextStream in(&file);
    data = in.readAll();
    file.close();
    return data;
}

bool Base::writeFileText(QString url, QString data)
{
    QFile file(url);
    if(!file.open(QIODevice::Text |QIODevice::WriteOnly)){
        emit sendMessage(url + tr(" 打开文件失败"),1);
        return false;
    }

    QTextStream in(&file);
    in << data;
    file.close();
    return true;
}

QString Base::timeToString(long long time)
{
    QDateTime dataTime;
    dataTime.setMSecsSinceEpoch(time);
    dataTime = dataTime.toUTC();
    return dataTime.toString("hh:mm:ss");
}

void Base::deskOpenFile(QString url, bool local)
{
    //检验本地文件
    if(!url.contains("file:///") && local){
        url = "file:///" + url;
    }

    QDesktopServices::openUrl(QUrl(url));
}

void Base::copyString(QString data)
{
    QClipboard* copy = QGuiApplication::clipboard();
    copy->setText(data);
}
