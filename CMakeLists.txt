cmake_minimum_required(VERSION 3.5)
project(MXD)

set(tools /usr/local/arm-linux-gnueabihf-4.9/usr)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Wpedantic -std=c++11")

add_subdirectory(src)


add_executable(tty_slave ${PROJECT_SOURCE_DIR}/src/tty_slave.cpp )
target_link_libraries(tty_slave serial)

add_executable(tty_master ${PROJECT_SOURCE_DIR}/src/tty_master.cpp )
target_link_libraries(tty_master serial)

add_executable(tty_slave_one ${PROJECT_SOURCE_DIR}/src/tty_slave_one.cpp )
target_link_libraries(tty_slave_one serial)

add_executable(tty_master_one ${PROJECT_SOURCE_DIR}/src/tty_master_one.cpp )
target_link_libraries(tty_master_one serial)
