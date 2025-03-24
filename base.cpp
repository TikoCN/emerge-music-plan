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

int Base::getChinesePinyingId(QString string)
{
    // 定义GB2312编码范围
    const int gb2312Range[][2] = {
        {0xA1A1, 0xA1F7}, // 标点符号等
        {0xB0A1, 0xB0F7}, {0xB1A1, 0xB1F7}, {0xB2A1, 0xB2F7}, {0xB3A1, 0xB3F7},
        {0xB4A1, 0xB4F7}, {0xB5A1, 0xB5F7}, {0xB6A1, 0xB6F7}, {0xB7A1, 0xB7F7},
        {0xB8A1, 0xB8F7}, {0xB9A1, 0xB9F7}, {0xBAA1, 0xBAF7}, {0xBBA1, 0xBBF7},
        {0xBCA1, 0xBCF7}, {0xBDA1, 0xBDF7}, {0xBEA1, 0xBEF7}, {0xBFA1, 0xBFF7},
        {0xC0A1, 0xC0F7}, {0xC1A1, 0xC1F7}, {0xC2A1, 0xC2F7}, {0xC3A1, 0xC3F7},
        {0xC4A1, 0xC4F7}, {0xC5A1, 0xC5F7}, {0xC6A1, 0xC6F7}, {0xC7A1, 0xC7F7},
        {0xC8A1, 0xC8F7}, {0xC9A1, 0xC9F7}, {0xCAA1, 0xCAF7}, {0xCBA1, 0xCBF7},
        {0xCCA1, 0xCCF7}, {0xCDA1, 0xCDF7}, {0xCEA1, 0xCEF7}, {0xCFA1, 0xCFF7},
        {0xD0A1, 0xD0F7}, {0xD1A1, 0xD1F7}, {0xD2A1, 0xD2F7}, {0xD3A1, 0xD3F7},
        {0xD4A1, 0xD4F7}, {0xD5A1, 0xD5F7}, {0xD6A1, 0xD6F7}, {0xD7A1, 0xD7F7},
        {0xD8A1, 0xD8F7}, {0xD9A1, 0xD9F7}, {0xDAA1, 0xDAF7}, {0xDBA1, 0xDBF7},
        {0xDCA1, 0xDCF7}, {0xDDA1, 0xDDF7}, {0xDEA1, 0xDEF7}, {0xDFA1, 0xDFF7}
    };

    string.toUtf8().toInt();
    return 0;
}
