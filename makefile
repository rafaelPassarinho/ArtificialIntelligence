#autor: devknot.asm@gmail.com

CC=clang

APP=ag

OBJ=#./obj/
BIN=#./bin/
SRC=#./src/
LIB=#./lib/
INC=#./include/

H=#-I$(INC)
L=#-L$(LIB)

DEP= algoritmoGenetico.o

CFLAGS= -O2 -lm

CXFLAGS= -c -static -O2 -D NDEBUG -Wall

#CXFLAGS= -c -g -D DEBUG -Wall

all: $(DEP)
	$(CC) $(CFLAGS) $(OBJ)*.o -o $(BIN)$(APP)

%.o: $(SRC)/%.c
	$(CC) $< $(CXFLAGS) $(H) $(L) -o $(OBJ)$@

clear:
	rm $(OBJ)*.o
	rm $(BIN)$(APP)

run:
	./$(APP)
	#bash run.sh
