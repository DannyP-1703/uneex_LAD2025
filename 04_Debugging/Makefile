TEST_DIR = tests
TRASH = *.o *~ o.* range
CFLAGS = -g -O0

all: range

clean:
	rm -rf $(TRASH)

test: range
	@for run in 1 2 ; do \
		if gdb -x script$${run}.gdb $< | grep @@@ | diff -q $(TEST_DIR)/test$${run} - ; then\
			echo "Test $${run} passed"; \
		fi \
	done

.PHONY: all clean test
