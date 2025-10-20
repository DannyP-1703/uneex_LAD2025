TEST_DIR = tests
TRASH = esub
CFLAGS = -g -O0 -Wall

all: esub

clean:
	rm -rf $(TRASH)

test: esub
	@echo "=== Testing unsuccessful returns ==="
	@./esub; if [ $$? -eq 1 ]; then echo "PASS"; else echo "FAIL"; fi
	@echo ""
	@./esub '[invalid' 'sub' 'test' ; if [ $$? -eq 2 ]; then echo "PASS"; else echo "FAIL"; fi
	@echo ""
	@./esub 'xyz' 'abc' 'test string' ; if [ $$? -eq 5 ]; then echo "PASS"; else echo "FAIL"; fi
	@echo ""
	@./esub 'a' '\l' 'abc' ; if [ $$? -eq 6 ]; then echo "PASS"; else echo "FAIL"; fi
	@echo ""
	@./esub 'a' '\2' 'abc' ; if [ $$? -eq 4 ]; then echo "PASS"; else echo "FAIL"; fi
	@echo ""
	
	@echo "=== Comparing output with sed ==="
	@for t in \
		'world|sed|hello world' \
		'(\w+) (\w+)|\2 \1|hello world' \
		'(\w+)(\d+)(\w+)|\3-\2-\1|abc123def' \
		'(a)b(c)d(e)|<\3\2\1>|_abcdef_' \
	 ; do \
		re=$$(printf '%s' "$$t" | cut -d'|' -f1); \
		sub=$$(printf '%s' "$$t" | cut -d'|' -f2); \
		str=$$(printf '%s' "$$t" | cut -d'|' -f3-); \
		printf '+ %s\n' "./esub '$$re' '$$sub' '$$str'"; \
		ESUB_OUT=$$(./esub "$$re" "$$sub" "$$str" 2>/dev/null) || ESUB_RC=$$?; \
		SED_OUT=$$(echo "$$str" | sed -E "s/$$re/$$sub/"); \
		if [ "$$ESUB_OUT" = "$$SED_OUT" ]; then \
			echo "PASS"; \
		else \
			echo "FAIL: re=$$re sub=$$sub esub='$$ESUB_OUT' sed='$$SED_OUT'"; \
		fi; \
	done

.PHONY: all clean test
