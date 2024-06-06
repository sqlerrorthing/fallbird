//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_STEALERIMPL_H
#define STEALER_STEALERIMPL_H
#include "../Includes.h"

class StealerImpl {
public:
    virtual void execute(fs::path& root) {};
    virtual ~StealerImpl() = default;
};


#endif //STEALER_STEALERIMPL_H
