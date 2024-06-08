//
// Created by TERETERY on 07.06.2024.
//

#ifndef STEALER_CRYPTO_H
#define STEALER_CRYPTO_H


#include "../../StealerModule.h"
#include "../../StealerModuleGroup.h"

class Crypto : public StealerModule, StealerModuleGroup {
public:
    Crypto();
    void execute(fs::path& root) override;
};


#endif //STEALER_CRYPTO_H
