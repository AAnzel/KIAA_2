PROGRAM = avl
CC = gcc
CFLAGS = -Wall -pedantic

$(PROGRAM): main.c avl_tree.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	-rm $(PROGRAM)
