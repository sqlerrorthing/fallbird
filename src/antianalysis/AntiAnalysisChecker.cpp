//
// Created by .1qxz on 11.06.2024.
//

#include "AntiAnalysisChecker.h"

bool AntiAnalysisChecker::check() {
    if(isHosting()) return false;

    return true;
}

bool AntiAnalysisChecker::isHosting() {
    try {
        std::pair<std::string, int> resp = HttpUtil::sendHttpRequest("http://ip-api.com/line/?fields=hosting", {
            {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
            {"Accept-Language", "en-US,en;q=0.9,ru;q=0.8"},
            {"Connection", "keep-alive"},
            {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36"},
        });

        if(resp.second != 200)
            return false;

        return resp.first.find("true") != std::string::npos;
    }
    catch (...)
    {

    }

    return false;
}
