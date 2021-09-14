#make file

PROJ_NAME = geneticAlgorithm

# .c files
C_SOURCE = $(wildcard *.c)

# Object files
OBJ =  $(C_SOURCE: .c = .o)

# Compiler
CC = gcc

# Flags for compiler
CC_FLAGS = -lm

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CC_FLAGS)

%.o: %.c
	$(CC) -o $@ $< $(CC_FLAGS)

.PHONY: all clean

clean:
	rm -rf *.o *~
	
run:
	./$(PROJ_NAME)
