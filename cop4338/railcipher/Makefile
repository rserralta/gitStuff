CC=gcc
CFLAGS=-I. -w -lm
OBJ = railcipher.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

railcipher: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
