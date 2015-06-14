#-DCMAKE_BUILD_TYPE=DEBUG

project(noolite-rx)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules) #include additional cmake modules(for finding MySQL, OpenCL libraries etc.)

find_package(USB REQUIRED)
include_directories(${LIBUSB_1_INCLUDE_DIRS})
add_definitions(-std=c++0x)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} defs.h defs_to_str.h loger.h)
target_link_libraries(${PROJECT_NAME} ${LIBUSB_1_LIBRARIES})
