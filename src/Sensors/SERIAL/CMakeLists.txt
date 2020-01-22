cmake_minimum_required(VERSION 3.6)
project(Serial)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Loom_Serial_Sensor.cpp Loom_Serial_Sensor.h)
add_executable(Serial ${SOURCE_FILES})