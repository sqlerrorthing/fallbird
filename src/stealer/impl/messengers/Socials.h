//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_SOCIALS_H
#define STEALER_SOCIALS_H

#include "../../StealerModule.h"
#include "../../StealerModuleGroup.h"

class Socials : public StealerModule, StealerModuleGroup {
public:
    Socials();
    void execute(fs::path& root) override;
};

#endif //STEALER_SOCIALS_H
