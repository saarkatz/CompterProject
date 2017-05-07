ODIR = obj
TEST_DIR = .
HOME_DIR = ..

CC = gcc
C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

_INCLUDE = $(TEST_DIR) $(HOME_DIR)
INCLUDEPATH = $(patsubst %,-I%,$(_INCLUDE))

_EXEC = sp_logger_unit_test
EXEC = $(patsubst %,$(TEST_DIR)/%,$(_EXEC))

_OBJS = sp_logger_unit_test.o SPLogger.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Define the test
_TEST = sp_logger_unit_test.c unit_test_util.h
_TEST_DEP = SPLogger.h
TEST = $(patsubst %,$(TEST_DIR)/%,$(_TEST)) $(patsubst %,$(HOME_DIR)/%,$(_TEST_DEP))

# Defien all the dependencies
_LOGGER_DEP = SPLogger.c SPLogger.h SPGlobals.h
LOGGER_DEP = $(patsubst %,$(HOME_DIR)/%,$(_LOGGER_DEP))

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
$(ODIR)/sp_logger_unit_test.o: $(TEST)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
$(ODIR)/SPLogger.o: $(LOGGER_DEP)
	$(CC) $(C_COMP_FLAG) $(INCLUDEPATH) -c -o $@ $<
clean:
	rm -f $(OBJS) $(EXEC)
