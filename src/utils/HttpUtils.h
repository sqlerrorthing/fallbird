//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_HTTPUTILS_H
#define STEALER_HTTPUTILS_H
#include "../Includes.h"
#include <windows.h>
#include <wininet.h>

class HttpUtils {
public:
    static std::pair<std::string, int> sendHttpRequest(const std::string& url, const std::string& headers = "");

};


#endif //STEALER_HTTPUTILS_H
