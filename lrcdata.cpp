#include "lrcdata.h"

int LrcData::getId() const
{
    return id;
}

void LrcData::setId(int newId)
{
    if (id == newId)
        return;
    id = newId;
    emit idChanged();
}

QList<long long> LrcData::getStartList() const
{
    return startList;
}

QList<long long> LrcData::getEndList() const
{
    return endList;
}

qint64 LrcData::getStartTime() const
{
    return startTime;
}

QList<QString> LrcData::getTextList() const
{
    return textList;
}

void LrcData::setTextList(const QList<QString> &newTextList)
{
    if (textList == newTextList)
        return;
    textList = newTextList;
    emit textListChanged();
}

bool LrcData::getIsPlay() const
{
    return isPlay;
}

void LrcData::setIsPlay(bool newIsPlay)
{
    if (isPlay == newIsPlay)
        return;
    isPlay = newIsPlay;
    emit isPlayChanged();
}

LrcData::LrcData(){
    id = 0;
    startTime = 0;
    endTime = 0;
    isPlay = false;
}

void LrcData::append(long long start, long long end, QString text)
{
    startList.append(start);
    endList.append(end);
    textList.append(text);
}




