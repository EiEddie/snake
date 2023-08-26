cmake_minimum_required(VERSION 3.19)
project(snake)


# 设定默认编译类型
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to \"Release\" as none was specified")
	set(CMAKE_BUILD_TYPE Release)

	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()


# 目标文件目录
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-D_DEBUG_)
	message(STATUS "Add macro \"_DEBUG_\"")
endif()

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE SRC_FILE "${PROJECT_SOURCE_DIR}/src/*")

if(SRC_FILE)
	add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILE})
	target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${CURSES_CFLAGS})
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
endif()
