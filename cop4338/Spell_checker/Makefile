CC=gcc
CFLAGS=-I. -w -lm
OBJ = hashset_pa4.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

spell_checker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
