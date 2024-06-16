//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_HTTPUTIL_H
#define STEALER_HTTPUTIL_H
#include "../Includes.h"
#include <windows.h>
#include <wininet.h>
#include <unordered_map>
#include <json.hpp>

using json = nlohmann::json;

class HttpUtil {
public:
    static std::pair<std::string, int> sendHttpRequest(const std::string& url, const std::unordered_map<std::string, std::string> &headers = {});
    static void sentPostRequest(const std::string &url, const json &data);
};


#endif //STEALER_HTTPUTIL_H
