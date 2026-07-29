#ifndef DATALOADER_H
#define DATALOADER_H

struct Loader {
    int  offset;
    int  limit;
    bool isFinish;

    Loader() {
        offset   = 0;
        limit    = 20;
        isFinish = false;
    }

    void reset() {
        offset   = 0;
        isFinish = false;
    }
};

#endif // DATALOADER_H
