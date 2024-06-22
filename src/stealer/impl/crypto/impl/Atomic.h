//
// Created by TERETERY on 22.06.2024.
//

#ifndef ATOMIC_H
#define ATOMIC_H


#include "../../../StealerModule.h"
class Atomic : public StealerModule {
public:
    void execute(fs::path &root) override;
};




#endif //ATOMIC_H
