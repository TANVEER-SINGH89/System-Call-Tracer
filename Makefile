CC=gcc
CFLAGS=-Wall

all:
	$(CC) src/main.c -o tracer $(CFLAGS)

run:
	./tracer /bin/ls

clean:
	rm -f tracer