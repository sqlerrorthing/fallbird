//
// Created by .1qxz on 12.06.2024.
//

#include "Counter.h"

unsigned __int64 Counter::history = 0;
std::mutex Counter::mtx;
unsigned __int64 Counter::cards = 0;
unsigned __int64 Counter::downloads = 0;
unsigned __int64 Counter::passwords = 0;
unsigned __int64 Counter::bookmarks = 0;
unsigned __int64 Counter::autoFills = 0;
unsigned __int64 Counter::cookies = 0;
bool Counter::minecraft = false;
unsigned __int64 Counter::crypto = 0;
unsigned __int64 Counter::files = 0;
bool Counter::battlenet = false;
bool Counter::steam = false;
bool Counter::ubisoftConnect = false;
bool Counter::telegram = false;
unsigned __int64 Counter::discordTokens = 0;

unsigned long long int Counter::getAutoFills() {
    return autoFills;
}

unsigned long long int Counter::getBookmarks() {
    return bookmarks;
}

unsigned long long int Counter::getCookies() {
    return cookies;
}

unsigned long long int Counter::getCards() {
    return cards;
}

unsigned long long int Counter::getDownloads() {
    return downloads;
}

unsigned long long int Counter::getHistory() {
    return history;
}

unsigned long long int Counter::getPasswords() {
    return passwords;
}

unsigned long long int Counter::getCrypto() {
    return crypto;
}

unsigned long long int Counter::getFiles() {
    return files;
}

bool Counter::isBattlenet() {
    return battlenet;
}

bool Counter::isMinecraft() {
    return minecraft;
}

bool Counter::isSteam() {
    return steam;
}

bool Counter::isUbisoftConnect() {
    return ubisoftConnect;
}

bool Counter::isTelegram() {
    return telegram;
}

unsigned long long int Counter::getDiscordTokens() {
    return discordTokens;
}

void Counter::increaseAutoFills() {
    std::lock_guard<std::mutex> lock(mtx);
    ++autoFills;
}

void Counter::increaseBookmarks() {
    std::lock_guard<std::mutex> lock(mtx);
    ++bookmarks;
}

void Counter::increaseCookies() {
    std::lock_guard<std::mutex> lock(mtx);
    ++cookies;
}

void Counter::increaseCards() {
    std::lock_guard<std::mutex> lock(mtx);
    ++cards;
}

void Counter::increaseDownloads() {
    std::lock_guard<std::mutex> lock(mtx);
    ++downloads;
}

void Counter::increaseHistory() {
    std::lock_guard<std::mutex> lock(mtx);
    ++history;
}

void Counter::increasePasswords() {
    std::lock_guard<std::mutex> lock(mtx);
    ++passwords;
}

void Counter::increaseCrypto() {
    std::lock_guard<std::mutex> lock(mtx);
    ++crypto;
}

void Counter::increaseFiles() {
    std::lock_guard<std::mutex> lock(mtx);
    ++files;
}

void Counter::setBattlenet() {
    std::lock_guard<std::mutex> lock(mtx);
    battlenet = true;
}

void Counter::setMinecraft() {
    std::lock_guard<std::mutex> lock(mtx);
    minecraft = true;
}

void Counter::setSteam() {
    std::lock_guard<std::mutex> lock(mtx);
    steam = true;
}

void Counter::setUbisoftConnect() {
    std::lock_guard<std::mutex> lock(mtx);
    ubisoftConnect = true;
}

void Counter::setTelegram() {
    std::lock_guard<std::mutex> lock(mtx);
    telegram = true;
}

void Counter::increaseDiscordTokens() {
    std::lock_guard<std::mutex> lock(mtx);
    ++discordTokens;
}