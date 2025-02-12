#include "popupdata.h"

PopupData::PopupData() {}

void PopupData::message(QString msg)
{
    emit cppMessage(msg);
}
