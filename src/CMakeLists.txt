cmake_minimum_required(VERSION 2.8.4)
project(SimpleHttpServer)

set(CMAKE_EXE_LINKER_FLAGS "-levent")

set(SOURCE_FILES main.cpp server.cpp)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(SimpleHttpServer ${SOURCE_FILES})