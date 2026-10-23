cmake_minimum_required(VERSION 3.15)
project(YADRO14_CORE CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(YADRO14_CORE main.cpp)

if(WIN32)
    target_link_libraries(YADRO14_CORE PRIVATE ws2_32)

    # Указываем CMake, где искать zlib
    # Ищем в папке, где лежит ваш компилятор (E:\CLion 2026.1.1\bin\mingw\x86_64-w64-mingw32)
    set(ZLIB_ROOT "${CMAKE_CXX_COMPILER}/../../x86_64-w64-mingw32")

    # Ищем библиотеку и заголовки
    find_package(ZLIB REQUIRED)

    # Привязываем zlib к проекту
    target_link_libraries(YADRO14_CORE PRIVATE ZLIB::ZLIB)
endif()