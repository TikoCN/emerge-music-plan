//
// Created by changzhi on 2026/2/16.
//

#ifndef MUSICPLAY_DELETE_H
#define MUSICPLAY_DELETE_H
#include "Update.h"

class Delete : public Update{
    Q_OBJECT
public:

    explicit Delete(TLog * log, BaseTool *tool) : Update(log, tool) {};

    bool deletePlayingList(int position);
};


#endif //MUSICPLAY_DELETE_H