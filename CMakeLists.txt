cmake_minimum_required(VERSION 3.16)
project(hashmap C)

set(CMAKE_C_STANDARD 99)
add_executable(hashmap library.h library.c main.c)

