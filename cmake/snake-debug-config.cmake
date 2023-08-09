cmake_minimum_required(VERSION 3.19)
project(snake-debug)


add_definitions(-D_DEBUG_)
message(STATUS "Add macro \"_DEBUG_\"")


include_directories("${PROJECT_SOURCE_DIR}/../src")

file(GLOB_RECURSE SRC_FILE "${PROJECT_SOURCE_DIR}/../src/*")
list(FILTER SRC_FILE EXCLUDE REGEX "main")


add_library(snake-debug ${SRC_FILE})
