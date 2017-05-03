#a rule for building a simple c++ source file
#use g++ -MM SPImageProc.cpp to see dependencies
#a rule for building a simple c source file
#use "gcc -MM SPPoint.c" to see the dependencies

CC = gcc
CPP = g++
#put all your object files here
OBJS = sp_config_unit_test.o SPConfig.o SPLogger.o
#The executabel filename
EXEC = sp_config_unit_test
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

HOME = ..
TEST = .

_INCLUDE = /usr/local/lib/opencv-3.1.0/include/  $(TEST) $(HOME)
INCLUDEPATH = $(foreach dir,$(_INCLUDE),-I$(dir))

CPP_COMP_FLAG = -std=c++11  -g -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -g -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
sp_config_unit_test.o: $(TEST)/sp_config_unit_test.c $(TEST)/unit_test_util.h $(HOME)/SPConfig.h $(HOME)/SPLogger.h $(HOME)/SPGlobals.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $*.c
SPConfig.o: $(HOME)/SPConfig.c $(HOME)/SPConfig.h $(HOME)/SPLogger.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPLogger.o: $(HOME)/SPLogger.c $(HOME)/SPLogger.h $(HOME)/SPGlobals.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
clean:
	rm -f $(OBJS) $(EXEC)
