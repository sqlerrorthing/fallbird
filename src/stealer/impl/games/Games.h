//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_GAMES_H
#define STEALER_GAMES_H


#include "../../StealerModule.h"
#include "../../StealerModuleGroup.h"

class Games : public StealerModule, StealerModuleGroup {
public:
    Games();
    void execute(fs::path &root) override;
};


#endif //STEALER_GAMES_H
