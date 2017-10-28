cmake_minimum_required(VERSION 2.8)

include_directories(${filemmap_SOURCE_DIR}/inc)

file(GLOB SRCS ${filemmap_SOURCE_DIR}/src/*.c)

add_library(filemmap SHARED file-mmap.c)

target_include_directories(filemmap PUBLIC ${CMAKE_CURRENT_LIST_DIR})
