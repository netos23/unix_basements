cmake_minimum_required(VERSION 3.20)

project(client C)
set(CMAKE_C_STANDARD 99)
add_executable(client src/client.c)

project(server C)
set(CMAKE_C_STANDARD 99)
add_executable(server src/server.c)