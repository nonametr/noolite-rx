project(noolite-rx)
cmake_minimum_required(VERSION 2.8.11)
site_name(HOSTNAME)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules) #include additional cmake modules(for finding MySQL, OpenCL libraries etc.)

#-DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
#-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

include_directories(./)

set(INC_LIST ${INC_LIST}
./common.h
./defs.h
./defs_to_str.h
./log.h
)

add_library(${PROJECT_NAME} ${CMAKE_LIBRARY_TYPE} ./rx2164.cpp)

find_package(USB REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries (${PROJECT_NAME} pthread ${LIBUSB_1_LIBRARIES})
add_definitions(-Wall -Werror -std=c++0x)

qt5_use_modules(${PROJECT_NAME} Core)
