# Circular Buffer and UART Device Driver Makefile
# Syntax $: make "Target" FRDM=1 //Default FRDM =0;

# -------------------------------------------
# From MCUXpresso
# FRDM Environment Makefile section.
ifdef FRDM
# (Pending)

# -------------------------------------------
# Linux Environment Makefile section.
else
SRC=src/
INC=inc/

CC=gcc
CFLAGS=-c -Wall -Werror -I $(INC)
CUFLAGS=-c - Wall -Werror -lcunit

all: ring CUnit

ring: ring.o
	$(CC) -o ring ring.o  		
	
ring.o: $(SRC)ring.c $(INC)ring.h
	$(CC) $(CFLAGS) $(SRC)ring.c

#CUnit: CUnit.o
#	$(CC) -o CUnit CUnit.o

#CUnit.o: #(SRC)ring_test.c $(SRC)ring.h
#	$(CC) $(CFLAGS) $(SRC)ring_test.c

#test: ring test.txt 
#		./ring<test.txt>outputFile.txt

clean:
	rm -rf *o ring
#	rm -rf outputFile.txt

endif
# Finish If Else statments. 