CC=gcc
LDIR=-L~/local/lib
IDIR=-I~/local/include/CUnit 
CFLAGS= -Wall -Werror
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/local/lib


Unit_Test: Unit_Test.o ring.o
	$(CC) $(CFLAGS) $(IDIR) Unit_Test.o ring.o $(LDIR) -lcunit -o Unit_Test
	

Manual_Test.o: Manual_Test.c ring.c ring.h
	$(CC) -c Manual_Test.c
	
	
Unit_Test.o: Unit_Test.c ring.c ring.h
	$(CC) -c Unit_Test.c

ring.o: ring.c ring.h
	$(CC) -c ring.c

clean:
	rm -rf *.o Unit_Test
	