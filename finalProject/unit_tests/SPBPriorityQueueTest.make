ODIR = obj
TEST_DIR = .
HOME_DIR = ..

CC = gcc
C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

_INCLUDE = $(TEST_DIR) $(HOME_DIR)
INCLUDEPATH = $(patsubst %,-I%,$(_INCLUDE))

_EXEC = sp_bpqueue_unit_test
EXEC = $(patsubst %,$(TEST_DIR)/%,$(_EXEC))

_OBJS = sp_bpqueue_unit_test.o SPBPriorityQueue.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Define the test
_TEST = sp_bpqueue_unit_test.c unit_test_util.h
_TEST_DEP = SPBPriorityQueue.h
TEST = $(patsubst %,$(TEST_DIR)/%,$(_TEST)) $(patsubst %,$(HOME_DIR)/%,$(_TEST_DEP))

# Defien all the dependencies
_QUEUE_DEP = SPBPriorityQueue.c SPBPriorityQueue.h
QUEUE_DEP = $(patsubst %,$(HOME_DIR)/%,$(_QUEUE_DEP))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
$(ODIR)/sp_bpqueue_unit_test.o: $(TEST)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPBPriorityQueue.o: $(QUEUE_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
clean:
	rm -f $(OBJS) $(EXEC)
