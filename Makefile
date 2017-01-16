CC=gcc
CFLAGS=-I.
DEPS = util.h
OBJ = bwt.o sa.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

src: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
