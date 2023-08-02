cmake_minimum_required(VERSION 3.19)
project(snake-test)


# 二进制输出目录
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")


set(snake-debug_DIR ${PROJECT_SOURCE_DIR}/../cmake)
find_package(snake-debug CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog CONFIG)

if(NOT spdlog_FOUND)
	add_definitions(-DNO_SPDLOG_LIB)
	message(STATUS "Add macro \"NO_SPDLOG_LIB\"")
endif()


function(add_test TEST_NAME)
	# 第一个参数作为测试的名字, 其余参数指定了测试文件
	# 将测试文件提取出来
	list(SUBLIST ARGV 1 -1 TEST_FILES)

	message(STATUS "Add a test named ${TEST_NAME}, files: ${TEST_FILES}")

	add_executable(${TEST_NAME} ${TEST_FILES})

	target_link_libraries(${TEST_NAME}
		snake-debug
		GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

	if(spdlog_FOUND)
		target_link_libraries(${TEST_NAME} spdlog::spdlog spdlog::spdlog_header_only)
	endif()
endfunction(add_test)


add_test(test_core
	test_core.cpp)
