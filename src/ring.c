/*****************************************************************************
* Copyright (C) 2019 by Ismail Yesildirek & Bijan Kianian
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Ismail Yesildirek, Bijan Kianian, and the University of Colorado are not
* liable for any misuse of this material.
*
*****************************************************************************/
/**
* @file ring.c
* @brief This source file contains a c program to implement a circular ring buffer.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 6 2019
* @version 1.4
*
*/

/*========================================================================*/
#include "ring.h"
/*========================================================================*/

/**######################################### init() - Start ###################################*/

ring_t *init ( uint32_t length)
{

	ring_t *p_Ring;

	char* array;

	if (length == 0)
	{
		printf("ERROR!... Buffer length cannot be 0.");
		return NULL;
	}

	array = (char*)calloc(length , sizeof(char));		//array is the actual 'ring buffer' with the size 'length' provided by client
	p_Ring = (ring_t*)malloc(sizeof(ring_t));		    //p_Ring is the allocated buffer structure defined by cient.

	p_Ring->Length = length; 	   								// initializing the buffer.
	p_Ring->Buffer = array;
	p_Ring->Ini = 0;
	p_Ring->Outi = 0;

	return p_Ring;													// returns the pointer which is pointing to the buffer.
}

/**######################################### init() - End ######################################*/

/****************************************** insert() -Start *****************************************/
int8_t insert ( ring_t *ring, char data )
{
	int8_t value = 0;
	if (ring == NULL)
	{
		value = -1;
		return value;
	}

	uint32_t Head = ring->Ini;
	uint32_t Tail = ring->Outi;
	uint32_t length = ring->Length-1;

		/********************************************************************************************************************
		    The head and tail pointers will remain always between 0 and [Length - 1]. If it equals to tail
			while the Buffer_Full flag is set, it means the buffer is full. The head will advance only
			when Buffer is not full.
			The Buffer_Full and Buffer_Empty flags help to indicate the status along with Head/Tail position.
			Once Head = Tail, depending which flag is set, it indicates the boundry violation, hence no waste of buffer space.
		 *********************************************************************************************************************/
	if (Buffer_Full == 0)
	{
		ring->Buffer[Head] = data;
		Head = (Head + 1) & length;
		Buffer_Empty = 0;							// As soon as an insert event, the Buffer_Empty flag shall reset.

		if(Head == Tail)
			Buffer_Full = 1;

		ring->Ini = Head &length;
	}

	else if ((Head == Tail ) && (Buffer_Full == 1))	// Buffer is full
		value = -2;
	else
		value = 0;

	return value;
}

/******************************************** insert()- End ************************************************/

/**------------------------------------------ read() -Start -----------------------------------------------**/


int8_t read ( ring_t *ring, char* data )
{
	int8_t value = 0;
	if (ring == NULL)
	{
		value = -1;
		return value;
	}

	uint32_t Head = ring->Ini;
	uint32_t Tail = ring->Outi;
	uint32_t length = ring->Length-1;

	if (Buffer_Empty == 0)
	{
		*data = ring->Buffer[Tail];
	   	Tail = (Tail+1) & length;
		Buffer_Full = 0;					// As soon as a read event, the Buffer_Full flag shall reset.

		if(Head == Tail)
			Buffer_Empty = 1;

	ring->Outi = Tail & length;
	value = 0;
	}

	else if ((Tail == Head) && (Buffer_Empty == 1))			// Buffer is empty
		value =  -2;

	else
		value =  0;

	return value;
}


/**----------------------------------------- read() - End -----------------------------------------**/

/**######################################## entries () - Start ####################################**/

int32_t entries ( ring_t *ring )
{
	uint32_t size, Head, Tail, length;

	if (ring == NULL)
		return -1;

	Head = ring->Ini & (ring->Length-1);
	Tail = ring->Outi & (ring->Length-1);
	length = ring->Length;

	size = length;							// When it's full, the size = length

	if(Buffer_Full == 0)
	{
		if	(Head >= Tail)
			size = Head - Tail;
		else
			size = length - Tail + Head;
	}

	return size;
}

/**######################################## entries() - End #######################################**/

/**$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Power_Of_Two() - Start $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$**/

/******************************************************************
  This is a check for 'length' value given by user,
  making sure it's a power of 2.
*******************************************************************/

uint8_t Power_Of_Two (uint32_t num)
{
	while (num !=1)
	{
		if(num%2 != 0)
			return 1;

		num = num / 2;
	}
	return 0;
}

/**$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Power_Of_Two() - End  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$**/

/**)))))))))))))))))))))))))))))))))) sizeValidation () - Start ))))))))))))))))))))))))))))))))))))**/

uint32_t sizeValidation (void)
{
	uint32_t size;
	char temp;							      // Used in FLUSH

	do							  /* Loop for chacking power of two input for length of buffer */
			{
				printf("Please enter the size of the buffer ( in powers of 2 ): ");
				FLUSH
				uint8_t validNumber = scanf("%d", &size);

				while((size <= 1)||(validNumber != 1))
				{
					printf("\nInvalid input! ");
					printf("Size must be a number greather than '1', try another number: ");
					FLUSH
					validNumber = scanf("%d", &size);
				}
				printf("\nYou entered: %d\n\n", size);

				if (Power_Of_Two(size) == 1)
					printf("Size must be in powers of 2 and greater than '1', please try again!\n");

			} while ( Power_Of_Two(size));

	return size;
}
/**)))))))))))))))))))))))))))))))))) sizeValidation () - End ))))))))))))))))))))))))))))))))))))))**/

/**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    display() - Start @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**/

void display ( ring_t* ring, int32_t Entries, char* data_out )
{

	uint32_t	Tail = ring->Outi,
				Head = ring->Ini,
				length = ring->Length;
	char	  * Buffer = ring->Buffer;


	if (data_out == NULL)
		printf("\nTail: %-5d	Head: %-5d	Entries: %-d	\n\nBuffer Contents Linear Queue (Tail to Head): ...",\
		Tail & (length - 1), Head &(length - 1), Entries);
	else
		printf("\nTail: %-5d	Head: %-5d	Entries: %-d	Data Out: %s \n\nBuffer Contents Linear Queue (Tail to Head): ...",\
		Tail & (length - 1), Head &(length - 1), Entries, data_out);

	for ( int32_t i = 0; i <=Entries-1; i++)
		putchar(*(Buffer + ((Tail + i) & (length-1))));

	printf("\nBuffer Contents Circular Queue: ...\"");

	for (int32_t i = 0; i<=length-1; i++)
		{
			if(Buffer[i] == '\0') Buffer[i] = '-' ;
			putchar(Buffer[i]);
		}

	putchar('\"');
}

/**;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; update_Buffer () - Start ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; **/

ring_t* update_Buffer (ring_t * Buffer)
{
	int32_t Entries = entries(Buffer);
	ring_t update_Buffer;
	ring_t* p_update_Buffer = &update_Buffer;
	p_update_Buffer->Ini = Buffer->Ini;
	p_update_Buffer->Outi = Buffer->Outi;
	p_update_Buffer->Length = Buffer->Length;

	uint32_t size = Buffer->Length;

	char circular_Q[MAX_LENGTH] = {'\0'};		      // Contains ring buffer elements in linear fashion, for presentation purpose.
	char tempbuffer[MAX_LENGTH] = {'\0'};		      // To save contents before resizing

	for (uint32_t i = 0 ; i < Entries ; i ++)
		tempbuffer[i] = *(Buffer->Buffer + ((Buffer->Outi + i) & (size-1)));

	for (uint32_t i = 0 ; i < size ; i ++)
		circular_Q [(Buffer->Outi + i) & (size-1)]= tempbuffer[i];

	p_update_Buffer->Buffer = circular_Q;

	return p_update_Buffer;
}
/**;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; update_Buffer () - Start ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; **/

/** ''''''''''''''''''''''''''''''' randomValue () - Start '''''''''''''''''''''''''''''''''**/

uint16_t randomValue ( uint32_t Seed, uint16_t modulus)   /**Linear Congruential Generator (LCG).**/
{

	uint32_t sizeList[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}; // Randomly selecting a number for buffer length

	const uint32_t a = 1664525;    /* Multiplier */
	const uint32_t c = 1013904223; /* Increment */
	uint32_t m = (uint32_t)pow(2,32);       /* Modulus */

	Seed = (a * Seed + c) % m;

	if (modulus == 10)
		return sizeList[Seed % 10];					//Will retun an elment from sizeList

	else if (modulus == MAX_LENGTH)
		return (Seed % MAX_LENGTH + 1);					//Will return a number betwee 1 and 1025.
	else if (modulus == 2)   						// "Max length is defined 1024 chars in this projects"
		return (Seed % 2);   						// 256 is for random ASCIi char generation used as input (char data_in[])
	else if (modulus == 256)
		return (Seed % 256);
	else return 0;
}
