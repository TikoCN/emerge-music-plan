#include "keyvaluepair.h"

KeyValuePair::KeyValuePair(QString key, QString value, QObject *parent)
    : QObject{parent}
{
    this->key = key;
    this->value = value;
}

QString KeyValuePair::getKey() const
{
    return key;
}

QString KeyValuePair::getValue() const
{
    return value;
}

