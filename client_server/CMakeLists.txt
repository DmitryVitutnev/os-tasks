cmake_minimum_required(VERSION 3.15)
project(client_server C)

set(CMAKE_C_STANDARD 99)


add_executable(client_server main.c)


find_package(Threads)
target_link_libraries(client_server ${CMAKE_THREAD_LIBS_INIT})