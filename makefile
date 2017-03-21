CC = gcc
CPP = g++
# put all your object files here
# The executabel filename
INCLUDEPATH=/usr/local/opt/opencv3/include/
LIBPATH=/usr/local/opt/opencv3/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

EXEC = SPCBIR
LOGGER_TEST_EXEC = SPCBIR_LOGGER_TESTS
CONFIG_TEST_EXEC = SPCBIR_CONFIG_TESTS
KD_ARRAY_TEST_EXEC = SPCBIR_KD_ARRAY_TESTS
all: $(EXEC) $(LOGGER_TEST_EXEC) $(CONFIG_TEST_EXEC) $(KD_ARRAY_TEST_EXEC)

OBJS = main.o main_aux.o SPConfig.o SPLogger.o SPPoint.o SPBPriorityQueue.o SPKDArray.o SPKDTree.o SPImageProc.o
$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
LOGGER_TEST_OBJS = sp_logger_unit_test.o SPLogger.o
$(LOGGER_TEST_EXEC): $(LOGGER_TEST_OBJS)
	$(CPP) $(LOGGER_TEST_OBJS) -o $@
CONFIG_TEST_OBJS = sp_config_unit_test.o SPConfig.o SPLogger.o
$(CONFIG_TEST_EXEC): $(CONFIG_TEST_OBJS)
	$(CPP) $(CONFIG_TEST_OBJS) -o $@
KD_ARRAY_TEST_OBJS = sp_kd_array_unit_test.o SPKDArray.o SPPoint.o
$(KD_ARRAY_TEST_EXEC): $(KD_ARRAY_TEST_OBJS)
	$(CPP) $(KD_ARRAY_TEST_OBJS) -o $@

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
sp_logger_unit_test.o: sp_logger_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_config_unit_test.o: sp_config_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_kd_array_unit_test.o: sp_kd_array_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC) $(LOGGER_TEST_OBJS) $(LOGGER_TEST_EXEC) $(CONFIG_TEST_EXEC) $(CONFIG_TEST_OBJS) $(KD_ARRAY_TEST_EXEC) $(KD_ARRAY_TEST_OBJS)
