cmake_minimum_required(VERSION 3.16)
project(hashmap C)

set(CMAKE_C_STANDARD 99)

add_library(hashmap library.c library.h)