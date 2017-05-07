ODIR = obj
TEST_DIR = .
HOME_DIR = ..

CC = gcc
C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

_INCLUDE = $(TEST_DIR) $(HOME_DIR)
INCLUDEPATH = $(patsubst %,-I%,$(_INCLUDE))

_EXEC = sp_kdtree_unit_test
EXEC = $(patsubst %,$(TEST_DIR)/%,$(_EXEC))

_OBJS = sp_kdtree_unit_test.o SPKDTree.o SPPoint.o SPKDArray.o SPBPriorityQueue.o SPLogger.o SPConfig.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Define the test
_TEST = sp_kdtree_unit_test.c unit_test_util.h
_TEST_DEP = SPKDTree.h SPConfig.h SPPoint.h SPKDArray.h SPBPriorityQueue.h SPLogger.h SPGlobals.h
TEST = $(patsubst %,$(TEST_DIR)/%,$(_TEST)) $(patsubst %,$(HOME_DIR)/%,$(_TEST_DEP))

# Defien all the dependencies
_KDTREE_DEP = SPKDTree.c SPKDTree.h SPPoint.h SPKDArray.h SPBPriorityQueue.h SPConfig.h SPLogger.h SPGlobals.h
KDTREE_DEP = $(patsubst %,$(HOME_DIR)/%,$(_KDTREE_DEP))

_POINT_DEP = SPPoint.c SPPoint.h 
POINT_DEP = $(patsubst %,$(HOME_DIR)/%,$(_POINT_DEP))

_KDARRAY_DEP = SPKDArray.c SPKDArray.h SPPoint.h SPLogger.h
KDARRAY_DEP = $(patsubst %,$(HOME_DIR)/%,$(_KDARRAY_DEP))

_QUEUE_DEP = SPBPriorityQueue.c SPBPriorityQueue.h
QUEUE_DEP = $(patsubst %,$(HOME_DIR)/%,$(_QUEUE_DEP))

_CONFIG_DEP = SPConfig.c SPConfig.h SPLogger.h
CONFIG_DEP = $(patsubst %,$(HOME_DIR)/%,$(_CONFIG_DEP))

_LOGGER_DEP = SPLogger.c SPLogger.h SPGlobals.h
LOGGER_DEP = $(patsubst %,$(HOME_DIR)/%,$(_LOGGER_DEP))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
$(ODIR)/sp_kdtree_unit_test.o: $(TEST)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPKDTree.o: $(KDTREE_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPPoint.o: $(POINT_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPKDArray.o: $(KDARRAY_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPBPriorityQueue.o: $(QUEUE_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPLogger.o: $(LOGGER_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPConfig.o: $(CONFIG_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
clean:
	rm -f $(OBJS) $(EXEC)
