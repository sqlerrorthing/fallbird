//
// Created by .1qxz on 07.06.2024.
//

#include "HttpUtil.h"

std::pair<std::string, int> HttpUtil::sendHttpRequest(const std::string &url, const std::unordered_map<std::string, std::string> &headers) {
    HINTERNET hSession = InternetOpen("HTTPS Client", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hSession) {
        return {"", -1};
    }

    // Формируем строку заголовков
    std::string headersStr;
    for (const auto &header : headers) {
        headersStr += header.first + ": " + header.second + "\r\n";
    }

    HINTERNET hConnect = InternetOpenUrl(hSession, url.c_str(), headersStr.c_str(), headersStr.length(), INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        return {"", -1};
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        response.append(buffer, bytesRead);
    }

    DWORD statusCode = 0;
    DWORD length = sizeof(statusCode);

    HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return {response, statusCode};
}
