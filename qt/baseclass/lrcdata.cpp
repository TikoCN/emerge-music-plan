#include "lrcdata.h"
#include <QJsonObject>
#include "base.h"

LrcData::LrcData(){
    id = 0;
    startTime = 0;
    endTime = 0;
    isPlay = false;
}

QJsonObject LrcData::getJsonObject()
{
    Base* base = Base::getInstance();
    QJsonObject json;
    json.insert("startTime", startTime);
    json.insert("endTime", endTime);
    json.insert("startList", base->intListToString(startList));
    json.insert("endList", base->intListToString(endList));
    json.insert("textList", base->stringListToString(textList));
    json.insert("helpTextList", base->stringListToString(helpTextList));
    return json;
}

void LrcData::append(long long start, long long end, QString text)
{
    startList.append(start);
    endList.append(end);
    textList.append(text);
}

void LrcData::copy(LrcDataPtr aim)
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




