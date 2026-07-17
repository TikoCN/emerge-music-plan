//
// Created by changzhi on 2026/2/16.
//

#ifndef MUSICPLAY_DELETE_H
#define MUSICPLAY_DELETE_H

#include "Core.h"

class Delete {
private:
    Core *core;

public:
    explicit Delete(Core *core) : core(core) {
    };
    [[nodiscard]] bool deletePlayingList(int position) const;
};


#endif //MUSICPLAY_DELETE_H
