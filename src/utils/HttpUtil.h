//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_HTTPUTIL_H
#define STEALER_HTTPUTIL_H
#include "../Includes.h"
#include <windows.h>
#include <wininet.h>

class HttpUtil {
public:
    static std::pair<std::string, int> sendHttpRequest(const std::string& url, const std::string& headers = "");

};


#endif //STEALER_HTTPUTIL_H
