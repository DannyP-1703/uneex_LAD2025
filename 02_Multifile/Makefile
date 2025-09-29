CC = gcc

TEST_DIR = test
GENERATES = prog prog-a prog-so liboutput_static.a liboutput.so
TRASH = *.o *~ o.* $(TEST_DIR)
CFLAGS = -Wall -fPIC

all: prog prog-a prog-so

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

fun.o prog.o: outlib.h

liboutput_static.a: fun.o const.o
	ar rcs $@ $^

liboutput.so: fun.o const.o
	$(CC) -shared $^ -o $@

prog: const.o fun.o prog.o
	$(CC) $^ -o $@

prog-a: prog.o liboutput_static.a
	$(CC) prog.o -o $@ -L. -loutput_static

prog-so: prog.o liboutput.so
	$(CC) prog.o -o $@ -L. -loutput -Wl,-rpath,.

clean:
	rm -rf $(TRASH) $(GENERATES)

test: prog prog-a prog-so
	@mkdir -p $(TEST_DIR)
	@for pr in $^ ; do \
		./$$pr > $(TEST_DIR)/$$pr.0.out 2>&1 ;\
		./$$pr arg1 > $(TEST_DIR)/$$pr.1.out 2>&1 ;\
		./$$pr arg1 arg2 arg 3 > $(TEST_DIR)/$$pr.3.out 2>&1 ;\
	done
	
	@for run in 0 1 3 ; do \
		cmp $(TEST_DIR)/prog.$$run.out $(TEST_DIR)/prog-a.$$run.out && cmp $(TEST_DIR)/prog-a.$$run.out $(TEST_DIR)/prog-so.$$run.out ;\
	done
	@echo "Passed tests"
