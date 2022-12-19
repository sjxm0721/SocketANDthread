cmake_minimum_required(VERSION 3.23)
project(Socket)

set(CMAKE_CXX_STANDARD 11)

link_libraries(pthread)

add_executable(Socket Server.cpp threadpool.cpp threadpool.h)
