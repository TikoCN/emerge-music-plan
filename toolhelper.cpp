#include "toolhelper.h"
#include "mediaplayer.h"
#include "extralibrary.h"
#include "popupdata.h"

ToolHelper::ToolHelper() {}

void ToolHelper::editMusic(int coreId)
{
    MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
    Music* core = mediaPlayer->coreList[coreId];
    QString lrcUrl = core->getLrcUrl();
    QFile file(lrcUrl);

    if(file.open(QIODevice::Text | QIODevice::ReadOnly)){
        QTextStream in(&file);
        emit cppEditMusic(in.readAll(), coreId);
    }
    file.close();
}

void ToolHelper::finishEdit(int coreId, QString lrc, QString title, QString artist, QString alumb, QString genre, QString year)
{
    MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
    Music* core = mediaPlayer->coreList[coreId];
    QString lrcUrl = core->getLrcUrl();
    QFile file(lrcUrl);

    if(file.open(QIODevice::Text | QIODevice::WriteOnly)){
        QTextStream in(&file);
        in << lrc;
    }
    file.close();

    QString url = core->url;
    ExtraLibrary extraLibrary;
    if(extraLibrary.setMedia(url, title, artist, alumb, genre, year)){
        //弹出通知
        PopupData* pop = PopupData::getInstance();
        pop->message("信息修改完成");
    }
}
