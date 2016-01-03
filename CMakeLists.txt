project(noolite-rx)
cmake_minimum_required(VERSION 2.8.11)
site_name(HOSTNAME)

add_executable(
    ${PROJECT_NAME}
    ./main.cpp
    ./rx2164.cpp
    )

include_directories(./)

set(INC_LIST ${INC_LIST}
./common.h
./defs.h
./defs_to_str.h
./log.h
)

find_package(FindUSB REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Threads REQUIRED)
qt5_use_modules(${PROJECT_NAME} Core)

target_link_libraries (${PROJECT_NAME} pthread)
add_definitions(-Wall -Werror -std=c++0x)
