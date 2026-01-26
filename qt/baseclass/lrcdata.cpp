#include "lrcdata.h"
#include "basetool/basetool.h"
#include <QJsonObject>
LrcData::LrcData()
    : id(0)
      , startTime(0)
      , endTime(0) {
}

QJsonObject LrcData::getJsonObject() const {
    QJsonObject json;
    json.insert("startTime", startTime);
    json.insert("endTime", endTime);
    json.insert("startList", TypeConversion::intListToString(startList));
    json.insert("endList", TypeConversion::intListToString(endList));
    json.insert("textList", TypeConversion::stringListToString(textList));
    json.insert("helpTextList", TypeConversion::stringListToString(helpTextList));
    return json;
}

void LrcData::append(const long long start, const long long end, const QString &text) {
    startList.append(start);
    endList.append(end);
    textList.append(text);
}

void LrcData::copy(const LrcDataPtr &aim) {
    this->id = aim->id;
    if (!aim->textList.empty() || !aim->helpTextList.empty()) {
        this->helpTextList = aim->helpTextList;
        this->textList = aim->textList;
        this->endList = aim->endList;
        this->startList = aim->startList;
        this->endTime = aim->endTime;
        this->startTime = aim->startTime;
    }
}
