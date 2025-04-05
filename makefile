# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
COMMON_SRCS = dataStruct.c io.c solver.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

MAIN_SRCS = main.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

#Solver Tester
SOLVER_TESTER_SRCS = Testers/solverTester.c
SOLVER_TESTER_OBJS = $(SOLVER_TESTER_SRCS:.c=.o)

#Board Tester
BOARD_TESTER_SRCS = Testers/boardTest.c
BOARD_TESTER_OBJS = $(BOARD_TESTER_SRCS:.c=.o)

#Dictionary Tester
DICTIONARY_TESTER_SRCS = Testers/dictionaryTest.c
DICTIONARY_TESTER_OBJS = $(DICTIONARY_TESTER_SRCS:.c=.o)

#Trie Node Tester
TRIE_NODE_TESTER_SRCS = Testers/trieNodeTest.c
TRIE_NODE_TESTER_OBJS = $(TRIE_NODE_TESTER_SRCS:.c=.o)


# Targets
all: clean main test

# Build MAIN executable (uses MAIN.o + common objects)
main: $(COMMON_OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o scrableSolver $(COMMON_OBJS) $(MAIN_OBJS)

# Build Solver tester executable
solverTester: $(COMMON_OBJS) $(SOLVER_TESTER_OBJS)
	$(CC) $(CFLAGS) -o solverTester $(COMMON_OBJS) $(SOLVER_TESTER_OBJS)

boardTester: $(BOARD_TESTER_OBJS)
	$(CC) $(CFLAGS) -o boardTester $(BOARD_TESTER_OBJS)

dictionaryTester: $(DICTIONARY_TESTER_OBJS)
	$(CC) $(CFLAGS) -o dictionaryTester $(COMMON_OBJS) $(DICTIONARY_TESTER_OBJS)

trieNodeTester: $(TRIE_NODE_TESTER_OBJS)
	$(CC) $(CFLAGS) -o trieNodeTester $(COMMON_OBJS) $(TRIE_NODE_TESTER_OBJS)

# Build all test executables
test: solverTester boardTester dictionaryTester trieNodeTester
	@echo "All test executables built."

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f *.o main ./Testers/*.o boardTester dictionaryTester trieNodeTester solverTester
