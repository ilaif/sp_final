cmake_minimum_required(VERSION 3.6)
project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_STANDARD 11)

#add_custom_target(final COMMAND make -C ${final_SOURCE_DIR} CLION_EXE_DIR=${PROJECT_BINARY_DIR})

set(SOURCE_FILES main.cpp SPLogger.h SPLogger.c SPConfig.h SPConfig.c)
add_executable(final ${SOURCE_FILES})

set(TEST_SOURCE_FILES sp_logger_unit_test.c SPLogger.h SPLogger.c SPConfig.h SPConfig.c)
add_executable(unit_tests ${TEST_SOURCE_FILES})
