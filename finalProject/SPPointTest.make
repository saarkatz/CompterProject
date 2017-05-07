ODIR = unit_tests/obj
TEST_DIR = ./unit_tests
HOME_DIR = .

CC = gcc
C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

_INCLUDE = $(TEST_DIR) $(HOME_DIR)
INCLUDEPATH = $(patsubst %,-I%,$(_INCLUDE))

EXEC = sp_point_unit_test

_OBJS = sp_point_unit_test.o SPPoint.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Define the test
_TEST = sp_point_unit_test.c unit_test_util.h
_TEST_DEP = SPPoint.h
TEST = $(patsubst %,$(TEST_DIR)/%,$(_TEST)) $(patsubst %,$(HOME_DIR)/%,$(_TEST_DEP))

# Defien all the dependencies
_POINT_DEP = SPPoint.c SPPoint.h 
POINT_DEP = $(patsubst %,$(HOME_DIR)/%,$(_POINT_DEP))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
$(ODIR)/sp_point_unit_test.o: $(TEST)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPPoint.o: $(POINT_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
clean:
	rm -f $(OBJS) $(EXEC)
