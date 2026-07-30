//
// Created by hrp on 2026/7/30.
//

#ifndef MUSICPLAY_QUEUEREPOSITORY_H
#define MUSICPLAY_QUEUEREPOSITORY_H

#include "Core.h"

class QueueRepository {
private:
    Core *core;

public:
    explicit QueueRepository(Core *core)
        : core(core) {
    };
    ~QueueRepository() = default;

    /**
     * 重新构建正在播放列表
     * @param ids 音乐id列表
     */
    void buildMusic(QList<int> ids) const;

    /**
     * 附加正在播放列表
     * @param ids 音乐id列表
     */
    void appendMusic(QList<int> ids) const;

    /**
     * 读取正在播放列表
     * @return 音乐id列表
     */
    [[nodiscard]] QList<int> getMusic() const;
};


#endif //MUSICPLAY_QUEUEREPOSITORY_H
