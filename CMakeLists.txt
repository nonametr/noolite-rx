cmake_minimum_required(VERSION 2.8)

project(noolite-trx)
add_executable(${PROJECT_NAME} "main.cpp" "rx2164.cpp" "tx1132.cpp" "common.h" "defs.h" "defs_to_str.h" "loger.h" "tcp_server.cpp" "tcp_server.h")

add_definitions(-std=gnu++0x)
target_link_libraries(${PROJECT_NAME} usb-1.0 pthread)
