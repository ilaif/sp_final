CC = gcc
CPP = g++
# put all your object files here
# TODO: SPImageProc.o
OBJS = main.o SPLogger.o SPPoint.o
LOGGER_TEST_OBJS = sp_logger_unit_test.o SPLogger.o
CONFIG_TEST_OBJS = sp_config_unit_test.o SPConfig.o SPLogger.o
# The executabel filename
EXEC = SPCBIR
LOGGER_TEST_EXEC = SPCBIR_LOGGER_TESTS
CONFIG_TEST_EXEC = SPCBIR_CONFIG_TESTS
INCLUDEPATH=/usr/local/opt/opencv3/include/
LIBPATH=/usr/local/opt/opencv3/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

all: $(EXEC) $(LOGGER_TEST_EXEC) $(CONFIG_TEST_EXEC)

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
$(LOGGER_TEST_EXEC): $(LOGGER_TEST_OBJS)
	$(CPP) $(LOGGER_TEST_OBJS) -L$(LIBPATH) $(LIBS) -o $@
$(CONFIG_TEST_EXEC): $(CONFIG_TEST_OBJS)
	$(CPP) $(CONFIG_TEST_OBJS) -L$(LIBPATH) $(LIBS) -o $@

# Main
main.o: main.cpp SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPConfig.h SPImageProc.cpp SPImageProc.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c


# Tests
sp_logger_unit_test.o: sp_logger_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_config_unit_test.o: sp_config_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC) $(LOGGER_TEST_OBJS) $(LOGGER_TEST_EXEC) $(CONFIG_TEST_EXEC)
