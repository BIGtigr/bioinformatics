CC=gcc
CFLAGS=-Isrc -Itest -Wall -Werror -O2

SRC_OBJ = obj/bwt.o obj/sa.o obj/util.o
TEST_OBJ = obj/sa_test.o obj/test_suite.o obj/util_test.o obj/bwt_test.o $(SRC_OBJ)

obj/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/%.o: test/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

src: $(SRC_OBJ) obj/main.o
	gcc -o bwt $^ $(CFLAGS)

test: $(TEST_OBJ)
	gcc -ggdb -o tests $^ $(CFLAGS)
	./tests

default: src
