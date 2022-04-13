cmake_minimum_required(VERSION 3.20)
project(s1 C)

set(CMAKE_C_STANDARD 99)

add_executable(s1 server1.c)

project(s2 C)

set(CMAKE_C_STANDARD 99)

add_executable(s2 server2.c)
