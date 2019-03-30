
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
* @file ring_test.c
* @brief The CUnit frame work
*
* @author Ismail Yesildirek, Bijan Kianian
* @date March 24 2019
* @version 1.0
*
*/
/**========================================================================*/
#include "ring.h"
/*========================================================================*/
ring_t RingBuffer;               //Global declaration of a buffer and it's pointer called RingBuffer/sample respectively.
ring_t *sample = &RingBuffer;

uint32_t length, old_length, Head, Tail;
int32_t Entries, old_Entries;
uint32_t read_out;				   // Number of characters to be read using read() function
char data_in[100] = {'\0'};		   // For insert test
char data_out[100] = {'\0'} ;				// For read test
char circular_Q[100] = {'\0'};		// Contains ring buffer elements in linear fashion, for presentation purpose.

char temp;							    	// Used in FLUSH
uint8_t startupFlag = 0;		// Used in buffer size change query
char cnt ='y';				// Flag for continue, or quit
char re_Size = 'y';
uint8_t init_flag = 1;		// Indicating whether the program is at the startup stage
//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((( init_suite_1() - Start )))))))))))))))))))))))))))))))))))))))))))))))))

int init_suite_1(void)
 {

	if(re_Size == 'y')
		{
			do										// Loop for chacking power of two input for length of buffer
			{
				old_length = length;
				printf("Please enter the size of the ring buffer ( in powers of 2 ): ");
				scanf("%d", &length);
				printf("You entered: %d\n\n", length);

				if( Power_Of_Two (length) == 1)
					printf(" Length is not a power of 2 number, please try again!\n");

			} while((Power_Of_Two(length)) && (re_Size == 'y'));
		}


		printf("Please enter a string for writing to the buffer:  ");
		FLUSH
		scanf("%[^\n]", data_in);
		printf("You entered: %s\n\n", data_in);

		printf("And the number of characters for removal: ");
	    FLUSH
		scanf("%d", &read_out);
		printf("You entered: %d\n\n", read_out);
		if( init_flag == 1)
		{
			sample = init(length);
			init_flag = 0;
			re_Size = 'n';
		}

		if (re_Size == 'y')
		{
			old_Entries = entries(sample);
			RingBuffer.Buffer = (char*)realloc(RingBuffer.Buffer, length * sizeof(char));
			
			Head = RingBuffer.Ini;
			Tail = RingBuffer.Outi;
			Entries = entries(sample);
			
			char temp_data;

			if (length >= old_length)
			{
				if(Head >= Tail)
				{
					for( uint32_t i = Head ; i < old_length ; i++ )
						*(sample->Buffer + (length - old_length) + i) = *(sample->Buffer + i);	// move data from head to the end of old size , to the new extension.

				Head = Head + (length - old_length);																// move head to the new extension but keep tail in same place.
				}
				else
				{
					for( uint32_t i = Tail ; i < old_length ; i++ )
					*(sample->Buffer + (length - old_length) + i) = *(sample->Buffer + i);		// move data from tail to the end of old size , to the new extension.

				Tail = Tail + (length - old_length);																	// move tail to the new extension but keep head in same place.
				}
			}
			else																														// Down sizing (lengh < old_length)
			{
				for (uint32_t i = 0 ; i < length ; i++ )
					{
						temp_data = *(sample->Buffer + (Tail + i)); 								    // move data from tail up to new length, back -
						RingBuffer.Buffer[i] = temp_data;													// - to the first index of buffer with new length.
					}

				if (Entries >= length)
					{ Head = length  ; Tail = 0; }
				else
					{ Head = Entries ; Tail = 0; }
			}

				sample->Outi = Tail ;
				sample->Ini = Head ;
		}

		sample->Length = length;

	return 0;
  }
//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((( init_suite_1() - End )))))))))))))))))))))))))))))))))))))))))))))))))

//################################## clean_suite_1() - Start #################################

int clean_suite_1(void)

{
	if (cnt != 'y')
		free(sample->Buffer);

	return 0;
}

//################################## clean_suite_1() - End #################################

//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ test_init_1() - Start }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

void test_init_1(void)
{
	if (init_flag == 1)
		CU_ASSERT(NULL != init(length));
}

//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ test_init_1() - End }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

// %%%%%%%%%%%%%%%%%%%%% test_insert_1() - Start %%%%%%%%%%%%%%%%%%%%

void test_insert_1(void)
{
	printf("\n\nPrevious Buffer Status:");

	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	if (re_Size == 'y')
	{
		re_Size = 'n';
		display( sample, old_length, old_Entries, NULL ) ;
	}		// data_out is not present when writing to the buffer, hence NULL
	else
	{
		Entries = entries( sample);
		display( sample, length,Entries, NULL);
	}
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	CU_ASSERT(-1 == insert(NULL, data_in[0]));					// Error checking for NULL pointer
	uint32_t count = strlen(data_in);

	for(uint16_t i = 0; i <= count-1; i++)
	{
		uint32_t check_insert = insert(sample, data_in[i]);
		CU_ASSERT(0 == check_insert);									//PASS
		if (check_insert == -2)														//Buffer Full
		{
			printf("\n\n!!!~~> Expected buffer overflow at location: ... %d\n", sample->Ini & (sample->Length-1));
			break;
		}
	}
	Entries = entries( sample);		//Update entries after intertion.
	display( sample, length, Entries, NULL ) ;
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}
// %%%%%%%%%%%%%%%%%%%%% test_insert_1() - End %%%%%%%%%%%%%%%%%%%%

/*************************************************** test_read_1() - Start ************************************************************/

void test_read_1(void)
{

	CU_ASSERT(-1 == read(NULL, data_out));		// Error checking for NULL pointer

	char data_out[100] = {'\0'};
	if (read_out != 0 )
	{
		for (uint16_t i = 0; i<=read_out-1; i++)
		{
			uint32_t check_read = read(sample, data_out+i);
			CU_ASSERT(0 == check_read);					//PASS
			if(check_read == -2)
			{
				printf("\n\n!!!~~> Expected buffer empty at location: ... %d\n", sample->Outi & (sample->Length-1));
				break;
			}
		}
	Entries = entries (sample);
	display( sample, length, Entries, data_out );
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	}
}
/***************************************************************** test_read_1() - End ************************************************/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ test_entries_1() - Start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void test_entries_1(void)
{
	CU_ASSERT(-1 == entries(NULL));
	CU_ASSERT(-1 != entries(sample));
	printf(" \nTail: %-5d	Head: %-5d	Entries: %-d...", sample->Outi & (sample->Length-1), sample->Ini & (sample->Length-1), entries( sample));
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ test_entries_1() - End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ########################################### main() - Start #######################################

int main (void)
{
		CU_pSuite pSuite1 = NULL;

		// Initialize CUnit test registry

		if (CUE_SUCCESS != CU_initialize_registry())
			return CU_get_error();

		// Add suite1 to registry

		pSuite1 = CU_add_suite("Basic_Test_Suite1\n", init_suite_1, clean_suite_1);
		if (NULL == pSuite1)
		{
			CU_cleanup_registry();
			return CU_get_error();
		}

		 // add test1 to suite1

		if ((NULL == CU_add_test(pSuite1,  " \n\n===> init() function:", test_init_1))||
			(NULL == CU_add_test(pSuite1,  " \n\n===> insert() function:", test_insert_1)) ||
		    (NULL == CU_add_test(pSuite1,  " \n\n===> read() function:", test_read_1)) ||
		    (NULL == CU_add_test(pSuite1,  " \n\n===> entries() function:", test_entries_1)))

		{
			CU_cleanup_registry();
			return CU_get_error();
		}

	while(cnt =='y')
		{
			CU_basic_set_mode(CU_BRM_VERBOSE);

			CU_basic_run_tests();									// OUTPUT to the screen

			printf("Continue? (Y)es, (N)o ");
			FLUSH
			scanf("%c", &cnt);
			scanf("%c", &temp);
			if (cnt =='y')
			{
				printf("Do you wish to modify the buffer size? (Y)es, (N)o: ");
				scanf("%c", &re_Size);
			}
		}


		CU_cleanup_registry();											// Cleaning the Registry
	return CU_get_error();
}

//########################################## main() - End #######################################
