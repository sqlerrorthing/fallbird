//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_TELEGRAM_H
#define STEALER_TELEGRAM_H

#include "../../../StealerModule.h"

class Telegram : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_TELEGRAM_H
