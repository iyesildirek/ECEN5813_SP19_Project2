CC=gcc
LDIR=-L~/local/lib
IDIR=-I~/local/include/CUnit 
CFLAGS= -Wall -Werror
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/local/lib

unittest: ring_test.o ring.o
	$(CC) $(CFLAGS) $(IDIR) ring_test.o ring.o $(LDIR) -lcunit -o unittest

ring_test.o: ring_test.c ring.c ring.h
	$(CC) -c ring_test.c

ring.o: ring.c ring.h
	$(CC) -c ring.c

clean:
	rm -rf *.o unittest
	