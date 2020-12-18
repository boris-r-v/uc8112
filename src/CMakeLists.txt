set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -std=c++11")
add_library(serial serial.cpp device.cpp)
