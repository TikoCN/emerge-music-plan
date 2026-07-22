//
// Created by changzhi on 2026/2/16.
//

#ifndef MUSICPLAY_DELETE_H
#define MUSICPLAY_DELETE_H

#include "Core.h"
#include <QList>
#include <QPair>

class Delete {
private:
    Core *core;

public:
    explicit Delete(Core *core) : core(core) {
    };
    [[nodiscard]] bool deletePlayingList(int position) const;
    [[nodiscard]] bool clearInvalidData() const;
    [[nodiscard]] bool clearInvalidData(const QList<int> &invalidMusicIds) const;
    [[nodiscard]] QList<QPair<int, QString>> getAllMusicData() const;
};


#endif //MUSICPLAY_DELETE_H
