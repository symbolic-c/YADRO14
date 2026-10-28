cmake_minimum_required(VERSION 3.15)
project(YADRO14_CORE CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем компилятору, что корень наших include — это папка src
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(YADRO14_CORE
        src/main.cpp
        src/core/Server.cpp
        src/core/PlayerManager.cpp
        src/network/NetworkManager.cpp
        src/world/FlatWorldGenerator.cpp
        src/world/World.cpp
        src/world/World.h
)

find_package(ZLIB REQUIRED)
target_link_libraries(YADRO14_CORE PRIVATE ZLIB::ZLIB)

if(WIN32)
    target_link_libraries(YADRO14_CORE PRIVATE ws2_32)
endif()