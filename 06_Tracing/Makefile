CC ?= gcc
CFLAGS ?= -Wall -Wextra

all: move libprotect.so

move: move.c
	$(CC) $(CFLAGS) -o $@ move.c

libprotect.so: protect.c
	$(CC) -fPIC -shared -o $@ protect.c -ldl

test: all
	@echo "Running tests..."
	bash tests/test.sh

clean:
	rm -f move libprotect.so

.PHONY: all test clean
