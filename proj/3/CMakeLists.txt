cmake_minimum_required(VERSION 3.20)
project(bs C)
set(CMAKE_C_STANDARD 99)
add_executable(bs src/bootstrap.c)

project(s C)
set(CMAKE_C_STANDARD 99)
add_executable(s src/server.c)

project(c C)
set(CMAKE_C_STANDARD 99)
add_executable(c src/client.c)
