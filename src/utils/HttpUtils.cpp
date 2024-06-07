//
// Created by .1qxz on 07.06.2024.
//

#include "HttpUtils.h"

std::pair<std::string, int> HttpUtils::sendHttpRequest(const std::string &url, const std::string &headers) {
    HINTERNET hSession = InternetOpen("HTTPS Client", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hSession) {
        return {"", -1};
    }

    HINTERNET hConnect = InternetOpenUrl(hSession, url.c_str(), headers.c_str(), headers.length(), INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        return {"", -1};
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;  // Null-terminate the buffer
        response.append(buffer, bytesRead);
    }

    DWORD statusCode = 0;
    DWORD length = sizeof(statusCode);

    HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return {response, statusCode};
}
