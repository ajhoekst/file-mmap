cmake_minimum_required(VERSION 2.8)
project(filemmap)

include_directories(${filemmap_SOURCE_DIR}/inc)

add_library(filemmap INTERFACE)
target_include_directories(filemmap INTERFACE ${filemmap_SOURCE_DIR}/inc)
