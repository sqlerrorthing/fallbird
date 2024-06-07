//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_STEALERMODULE_H
#define STEALER_STEALERMODULE_H
#include "../Includes.h"

class StealerModule {
public:
    virtual void execute(fs::path& root) {};
    virtual ~StealerModule() = default;
};


#endif //STEALER_STEALERMODULE_H
