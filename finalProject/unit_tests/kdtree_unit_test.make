#a rule for building a simple c++ source file
#use g++ -MM SPImageProc.cpp to see dependencies
#a rule for building a simple c source file
#use "gcc -MM SPPoint.c" to see the dependencies

CC = gcc
CPP = g++
#put all your object files here
OBJS = sp_kdtree_unit_test.o SPKDTree.o SPPoint.o SPKDArray.o SPBPriorityQueue.o SPLogger.o SPConfig.o
#The executabel filename
EXEC = sp_kdtree_unit_test
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

HOME = ..
TEST = .

_INCLUDE = /usr/local/lib/opencv-3.1.0/include/  $(TEST) $(HOME)
INCLUDEPATH = $(foreach dir,$(_INCLUDE),-I$(dir))

CPP_COMP_FLAG = -g -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -g -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
sp_kdtree_unit_test.o: $(TEST)/sp_kdtree_unit_test.c $(TEST)/unit_test_util.h $(HOME)/SPKDTree.h $(HOME)/SPConfig.h $(HOME)/SPPoint.h $(HOME)/SPKDArray.h $(HOME)/SPBPriorityQueue.h $(HOME)/SPLogger.h $(HOME)/SPGlobals.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $*.c
SPKDTree.o: $(HOME)/SPKDTree.c $(HOME)/SPKDTree.h $(HOME)/SPPoint.h $(HOME)/SPKDArray.h $(HOME)/SPBPriorityQueue.h $(HOME)/SPConfig.h $(HOME)/SPLogger.h $(HOME)/SPGlobals.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPPoint.o: $(HOME)/SPPoint.c $(HOME)/SPPoint.h 
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPKDArray.o: $(HOME)/SPKDArray.c $(HOME)/SPKDArray.h $(HOME)/SPPoint.h $(HOME)/SPLogger.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPBPriorityQueue.o: $(HOME)/SPBPriorityQueue.c $(HOME)/SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPLogger.o: $(HOME)/SPLogger.c $(HOME)/SPLogger.h $(HOME)/SPGlobals.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
SPConfig.o: $(HOME)/SPConfig.c $(HOME)/SPConfig.h $(HOME)/SPLogger.h
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c $(HOME)/$*.c
clean:
	rm -f $(OBJS) $(EXEC)
