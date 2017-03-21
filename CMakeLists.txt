cmake_minimum_required(VERSION 3.6)
project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_STANDARD 11)

#add_custom_target(final COMMAND make -C ${final_SOURCE_DIR} CLION_EXE_DIR=${PROJECT_BINARY_DIR})

set(INCLUDEPATH /usr/local/opt/opencv3/include/)
set(LIBPATH /usr/local/opt/opencv3/lib/)
set(LIBS -lopencv_xfeatures2d -lopencv_features2d -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core)

include_directories(${INCLUDEPATH})
link_directories(${LIBPATH})

set(SOURCE_FILES main.cpp main_aux.h main_aux.cpp SPLogger.h SPLogger.c SPConfig.h SPConfig.c SPPoint.h SPPoint.c
        SPKDArray.h SPKDArray.c SPKDTree.h SPKDTree.c SPBPriorityQueue.h SPBPriorityQueue.c SPImageProc.h SPImageProc.cpp)
add_executable(final ${SOURCE_FILES})

target_link_libraries(final ${LIBS})

set(TEST_LOGGER sp_logger_unit_test.c SPLogger.h SPLogger.c)
add_executable(test_logger ${TEST_LOGGER})

set(TEST_CONFIG sp_config_unit_test.c SPLogger.h SPLogger.c SPConfig.h SPConfig.c)
add_executable(test_config ${TEST_CONFIG})

set(TEST_KD_ARRAY sp_kd_array_unit_test.c SPKDArray.h SPKDArray.c SPPoint.h SPPoint.c)
add_executable(test_kd_array ${TEST_KD_ARRAY})