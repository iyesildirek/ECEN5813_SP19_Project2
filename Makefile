# Reference http://nuclear.mutantstargoat.com/articles/make/ 

CC=gcc
# -Iinc for header file location
CFLAGS=-Wall -Werror -Iinc

# Source file location
src = $(wildcard src/*.c)
srcTest = $(wildcard Unittest/*.c)

# Generate object files
obj = $(src:.c=.o) $(srcTest:.c=.o)

CUnit: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) -lcunit

.PHONY: clean

clean:
	rm -f $(obj) CUnit

