#make file

CC = gcc
CFLAGS = -lm
TARGET = algoritmoGenetico

all: $(TARGET).o
	$(CC) -o $(TARGET) $(TARGET).c $(CFLAGS)


clean:
	rm *o
	
run:
	./$(TARGET)
