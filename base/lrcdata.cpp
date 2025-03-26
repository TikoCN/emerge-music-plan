#include "lrcdata.h"

int LrcData::getId() const
{
    return id;
}

QList<long long> LrcData::getStartList() const
{
    return startList;
}

QList<long long> LrcData::getEndList() const
{
    return endList;
}

QList<QString> LrcData::getTextList() const
{
    return textList;
}

QList<QString> LrcData::getHelpTextList() const
{
    return helpTextList;
}

long long LrcData::getStartTime() const
{
    return startTime;
}

long long LrcData::getEndTime() const
{
    return endTime;
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

void LrcData::copy(LrcData *aim)
{
    this->id = aim->id;
    if(aim->textList.size() > 0 || aim->helpTextList.size() > 0){
        this->helpTextList = aim->helpTextList;
        this->textList = aim->textList;
        this->endList = aim->endList;
        this->startList = aim->startList;
        this->endTime = aim->endTime;
        this->startTime = aim->startTime;
    }
}




