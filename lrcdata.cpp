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

int LrcData::getLine() const
{
    return line;
}

void LrcData::setLine(int newLine)
{
    if (line == newLine)
        return;
    line = newLine;
    emit lineChanged();
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

LrcData::LrcData(){
    id = 0;
    startTime = 0;
    endTime = 0;
    isPlay = false;
    line = 0;
    pos = 0.0;
}

double LrcData::getPos() const
{
    return pos;
}

void LrcData::setPos(double newPos)
{
    if (qFuzzyCompare(pos, newPos))
        return;
    pos = newPos;
    emit posChanged();
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

QString LrcData::getText() const
{
    return text;
}

void LrcData::setText(const QString &newText)
{
    if (text == newText)
        return;
    text = newText;
    setLine(text.split("\n").size());
    emit textChanged();
}

