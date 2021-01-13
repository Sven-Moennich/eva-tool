CC=gcc
CFLAGS=-Wall

.PHONY : all tools eva-tool clean

all: tools socket eva-tool
	$(CC) -o eva-tool eva-tool.o tools.o socket.o -levaftp

socket:
	$(CC) -c socket.c

tools:
	$(CC) -c tools.c

eva-tool:
	$(CC) -c eva-tool.c

clean:
	rm -f *.o eva-tool
	
