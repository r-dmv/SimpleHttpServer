cmake_minimum_required(VERSION 2.8.4)
project(SimpleHttpServer)

set(SOURCE_FILES main.cpp server.cpp HttpRequest.cpp utils.cpp HttpResponse.cpp)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(SimpleHttpServer ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(SimpleHttpServer event)