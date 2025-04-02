#include "base.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

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
        sendMessage(url + tr(" 文件不存在"),1);
        return data;
    }

    if(!file.open(QIODevice::Text |QIODevice::ReadOnly)){
        sendMessage(url + tr(" 打开文件失败"),1);
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
        sendMessage(url + tr(" 打开文件失败"),1);
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

QList<int> Base::getRandNumber(int min, int max, int count)
{
    QSet<int> randSet;

    if (max - min < count) {
        return QList<int> ();
    }
    while (randSet.size() < count) {
        int n = QRandomGenerator::global()->bounded(max - min) + min;
        randSet.insert(n);
    }

    return randSet.values();
}

QList<QStringList> Base::getChineseToPinyinJson()
{
    QFile dataFile(QDir::currentPath() + "/EnglishId.json");
    // 打开文件失败，退出读写
    if(!dataFile.open(QIODevice::Text | QIODevice::ReadOnly)){
        dataFile.close();
        return QList<QStringList> ();
    }

    // 执行写入文件
    QJsonDocument doc;
    doc = doc.fromJson(dataFile.readAll());
    QJsonObject obj = doc.object();
    dataFile.close();
    QList<QStringList> list;
    for (char i = 'a'; i <= 'z'; ++i) {
        QStringList stringList = obj.value(QString(i)).toString().split("");
        list.append(stringList);
    }

    return list;
}

void Base::sendMessage(QString msg, int type)
{
    emit message(msg, type);
}

bool Base::renameFile(QString oldUrl, QString newUrl)
{
    try{
        QFile oldFile(oldUrl);
        QFile newFile(newUrl);

        if(!oldFile.exists()){
            throw QString(oldUrl + tr("文件不存在"));
        }

        if (!oldFile.setPermissions(QFile::WriteOther)) {
            throw QString(oldUrl + "获取权限失败," + oldFile.errorString());
        }

        // 移除以及存在文件
        if (newFile.exists()) {

            if (!newFile.setPermissions(QFile::WriteOther)) {
                throw QString(newUrl + "获取权限失败," + newFile.errorString());
            }

            if(!newFile.remove()){
                throw QString(newUrl + tr("文件已经存在且无法删除,") + newFile.errorString());
            }
        }

        if(!oldFile.rename(oldUrl, newUrl)){
            throw QString(oldUrl + tr("重命名失败,") + oldFile.errorString());
        }
    }
    catch(QString e){
        qDebug()<<e;
        return false;
    }
    return true;
}
