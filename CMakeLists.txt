cmake_minimum_required(VERSION 3.5)
project(MXD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Wpedantic -std=c++11")

add_subdirectory(src)

add_executable(tty_slave ${PROJECT_SOURCE_DIR}/src/tty_slave.cpp )
target_link_libraries(tty_slave serial)

add_executable(tty_master ${PROJECT_SOURCE_DIR}/src/tty_master.cpp )
target_link_libraries(tty_master serial)


