CC = gcc
CPP = g++
# put all your object files here
# TODO: SPImageProc.o
OBJS = main.o SPLogger.o SPPoint.o
TEST_OBJS = sp_logger_unit_test.o SPLogger.o
# The executabel filename
EXEC = SPCBIR
TEST_EXEC = SPCBIR_TESTS
INCLUDEPATH=/usr/local/opt/opencv3/include/
LIBPATH=/usr/local/opt/opencv3/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
$(TEST_EXEC): $(TEST_OBJS)
	$(CPP) $(TEST_OBJS) -L$(LIBPATH) $(LIBS) -o $@

# Main
main.o: main.cpp SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPConfig.h SPImageProc.cpp SPImageProc.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h
	$(CC) $(C_COMP_FLAG) -c $*.c


# Tests
sp_logger_unit_test.o: sp_logger_unit_test.c unit_test_util.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC) $(TEST_OBJS) $(TEST_EXEC)
