cmake_minimum_required(VERSION 3.8)
project(CANWrapTest)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c CanWrap.h CanWrap.c)
add_executable(CANWrapTest ${SOURCE_FILES})