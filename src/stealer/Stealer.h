//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_STEALER_H
#define STEALER_STEALER_H
#include "../Includes.h"
#include "StealerImpl.h"

class Stealer {
public:
    Stealer();
    void run();
private:
    fs::path root_dir;
    std::list<StealerImpl*> modules;
};


#endif //STEALER_STEALER_H
