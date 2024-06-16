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

void HttpUtil::sentPostRequest(const std::string &url, const nlohmann::json &data) {
    std::string jsonData = data.dump();

    HINTERNET hSession = InternetOpen("HTTP_POST",
                                      INTERNET_OPEN_TYPE_DIRECT,
                                      NULL,
                                      NULL,
                                      0);

    if (!hSession) {
        return;
    }

    URL_COMPONENTS urlComponents = {0};
    urlComponents.dwStructSize = sizeof(urlComponents);
    char hostName[256];
    char urlPath[256];
    urlComponents.lpszHostName = hostName;
    urlComponents.dwHostNameLength = _countof(hostName);
    urlComponents.lpszUrlPath = urlPath;
    urlComponents.dwUrlPathLength = _countof(urlPath);

    if (!InternetCrackUrl(url.c_str(), 0, 0, &urlComponents)) {
        InternetCloseHandle(hSession);
        return;
    }

    HINTERNET hConnect = InternetConnect(hSession,
                                         urlComponents.lpszHostName,
                                         urlComponents.nPort,
                                         NULL,
                                         NULL,
                                         INTERNET_SERVICE_HTTP,
                                         0,
                                         0);

    if (!hConnect) {
        InternetCloseHandle(hSession);
        return;
    }

    DWORD dwFlags = INTERNET_FLAG_RELOAD;
    if (urlComponents.nScheme == INTERNET_SCHEME_HTTPS) {
        dwFlags |= INTERNET_FLAG_SECURE;
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect,
                                         "POST",
                                         urlComponents.lpszUrlPath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         dwFlags,
                                         0);

    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return;
    }

    if (!HttpAddRequestHeaders(hRequest, "Content-Type: application/json", -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE)) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return;
    }

    HttpSendRequest(hRequest, NULL, 0, (LPVOID)jsonData.c_str(), jsonData.size());

    // Очистка
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
}
