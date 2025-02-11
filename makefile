# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
COMMON_SRCS = dataStruct.c io.c solver.c 
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

MAIN_SRCS = main.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

TESTER_SRCS = tester.c
TESTER_OBJS = $(TESTER_SRCS:.c=.o)

# Targets
all: MAIN tester

# Build MAIN executable (uses MAIN.o + common objects)
main: $(COMMON_OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o main $(COMMON_OBJS) $(MAIN_OBJS)

# Build tester executable (uses tester.o + common objects, but NOT MAIN.o)
tester: $(COMMON_OBJS) $(TESTER_OBJS)
	$(CC) $(CFLAGS) -o tester $(COMMON_OBJS) $(TESTER_OBJS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f *.o main tester
