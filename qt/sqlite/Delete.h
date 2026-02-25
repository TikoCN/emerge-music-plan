//
// Created by changzhi on 2026/2/16.
//

#ifndef MUSICPLAY_DELETE_H
#define MUSICPLAY_DELETE_H
#include <QObject>
#include "Update.h"

class Delete : public Update{
    Q_OBJECT
public:

    explicit Delete(TLog * log) : Update(log) {};

    bool deletePlayingList(int position);
};


#endif //MUSICPLAY_DELETE_H