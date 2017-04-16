CC = gcc
CPP = g++
# put all your object files here
# The executabel filename
INCLUDEPATH=/usr/local/opt/opencv3/include/
LIBPATH=/usr/local/opt/opencv3/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

CPP_COMP_FLAG = -std=c++11 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
C_COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

EXEC = SPCBIR
CONFIG_TEST_EXEC = unit_tests/SPCBIR_CONFIG_TESTS
KD_ARRAY_TEST_EXEC = unit_tests/SPCBIR_KD_ARRAY_TESTS
BPQUEUE_TEST_EXEC = unit_tests/SPCBIR_BPQUEUE_TESTS
KD_TREE_TEST_EXEC = unit_tests/SPCBIR_KD_TREE_TESTS
POINT_TEST_EXEC = unit_tests/SPCBIR_POINT_TESTS
all: $(EXEC) $(CONFIG_TEST_EXEC) $(KD_ARRAY_TEST_EXEC) $(BPQUEUE_TEST_EXEC) $(KD_TREE_TEST_EXEC) $(POINT_TEST_EXEC)

OBJS = main.o main_aux.o SPConfig.o SPLogger.o SPPoint.o SPBPriorityQueue.o SPKDArray.o SPKDTree.o SPImageProc.o
$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
CONFIG_TEST_OBJS = unit_tests/sp_config_unit_test.o SPConfig.o SPLogger.o
$(CONFIG_TEST_EXEC): $(CONFIG_TEST_OBJS)
	$(CPP) $(CONFIG_TEST_OBJS) -o $@
KD_ARRAY_TEST_OBJS = unit_tests/sp_kd_array_unit_test.o SPKDArray.o SPPoint.o
$(KD_ARRAY_TEST_EXEC): $(KD_ARRAY_TEST_OBJS)
	$(CPP) $(KD_ARRAY_TEST_OBJS) -o $@
KD_TREE_TEST_OBJS = unit_tests/sp_kd_tree_unit_test.o SPKDTree.o SPPoint.o SPBPriorityQueue.o SPConfig.o SPKDArray.o SPLogger.o
$(KD_TREE_TEST_EXEC): $(KD_TREE_TEST_OBJS)
	$(CPP) $(KD_TREE_TEST_OBJS) -o $@
KD_BPQUEUE_TEST_OBJS = unit_tests/sp_bpqueue_unit_test.o SPBPriorityQueue.o
$(BPQUEUE_TEST_EXEC): $(KD_BPQUEUE_TEST_OBJS)
	$(CPP) $(KD_BPQUEUE_TEST_OBJS) -o $@
POINT_TEST_OBJS = unit_tests/sp_point_unit_test.o SPPoint.o
$(POINT_TEST_EXEC): $(POINT_TEST_OBJS)
	$(CPP) $(POINT_TEST_OBJS) -o $@

# Main
main.o: main.cpp
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
main_aux.o: main_aux.h main_aux.cpp
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPConfig.h SPImageProc.cpp SPImageProc.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDArray.o: SPKDArray.c SPKDArray.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDTree.o: SPKDTree.c SPKDTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c

# Tests
sp_config_unit_test.o: ./unit_tests/sp_config_unit_test.c ./unit_tests/unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_kd_array_unit_test.o: ./unit_tests/sp_kd_array_unit_test.c ./unit_tests/unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_bpqueue_unit_test.o: ./unit_tests/sp_bpqueue_unit_test.c ./unit_tests/unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_kd_tree_unit_test.o: ./unit_tests/sp_kd_tree_unit_test.c ./unit_tests/unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_point_unit_test.o: ./unit_tests/sp_point_unit_test.c ./unit_tests/unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC) $(CONFIG_TEST_EXEC) $(CONFIG_TEST_OBJS) $(KD_ARRAY_TEST_EXEC) $(KD_ARRAY_TEST_OBJS)
