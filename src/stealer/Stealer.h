//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_STEALER_H
#define STEALER_STEALER_H
#include "../Includes.h"
#include "StealerModule.h"
#include "StealerModuleGroup.h"

class Stealer : StealerModuleGroup {
public:
    Stealer();

    void run();
    void complete();
private:
    void registerModules();

    fs::path root_dir;
};


#endif //STEALER_STEALER_H
