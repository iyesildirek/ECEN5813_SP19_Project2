
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
* @file Unit_Test.c
* @brief The CUnit frame work
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 6 2019
* @version 1.6
*
*/

#define AUTO 0

/*========================================================================*/
#include "ring.h"
/*========================================================================*/

ring_t *sample;
ring_t *p_old_values;
ring_t *p_user1;
ring_t *p_user2;
ring_t *Ascii_buffer;

uint32_t old_length, Head, Tail;
uint32_t length = 0;
int32_t Entries, old_Entries;
uint32_t read_out;				  		  // Number of characters to be read using read() function
uint32_t seed = 0;							  // Seed value for random# function
char data_in   [MAX_LENGTH] = {'\0'};		 	  // For insert test
char data_out  [MAX_LENGTH] = {'\0'};			  // For read test
char tempbuffer[MAX_LENGTH] = {'\0'};		      // To save contents before resizing

char temp;							      // Used in FLUSH

#if !AUTO
char cnt ='y';				              // Flag for continue, or quit
char re_Size = 'y';
#else
uint8_t cnt = 1;				              // Flag for continue, or quit
uint8_t  re_Size = 1;
#endif

uint8_t init_flag = 1;		              // Indicating whether the program is at the startup stage

/** ((((((((((((((((((((((((((((((((((( init_suite() - Start ))))))))))))))))))))))))))))))))))) **/

int init_suite(void)
 {
#if !AUTO
	if(init_flag == 1)
	{
		printf("\t\t#################################\n");
		printf("\t\t#  Circular Buffer Manual Test 	#\n");
		printf("\t\t#################################\n\n");
	}

	if(re_Size == 'y')
		{
#else

	if(re_Size == 1)
		{
#endif

#if !AUTO
			do							  // Loop for chacking power of two input for length of buffer
			{

#else
				old_length = length;
				seed++;
				length = randomValue(seed,10);
#endif

#if !AUTO

				old_length = length;
				printf("Please enter the size of the buffer ( in powers of 2 ): ");
				uint8_t validNumber = scanf("%d", &length);

				while((length <= 1)||(validNumber != 1))
				{
					printf("\nInvalid input! ");
					printf("Size must be a number greather than '1', try another number: ");
					FLUSH
					validNumber = scanf("%d", &length);
				}
				printf("\nYou entered: %d\n\n", length);

				if (Power_Of_Two(length) == 1)
					printf("Size must be in powers of 2 and greater than '1', please try again!\n");

			} while((Power_Of_Two(length)) && (re_Size == 'y'));
		}
#else
		}
		seed++;
		uint16_t strSize = randomValue(seed, MAX_LENGTH);		// Random # between 0 ~ 1024 for string length

		for( uint16_t l = 1; l <= strSize ; l++)				// data_in holds the generated string with random length
			data_in[l] = randomValue(data_in[l-1], 256);
#endif

#if !AUTO
		printf("Type a string for writing to the buffer:  ");
		FLUSH
		scanf("%[^\n]", data_in);
		printf("\nYou entered: %s\n\n", data_in);

		printf("And the number of characters for removal: ");
	    FLUSH
		scanf("%d", &read_out);
		printf("\nYou entered: %d\n\n", read_out);

		if( init_flag == 1)
		{
			sample = init(length);
			init_flag = 0;
			re_Size = 'n';
		}

		if (re_Size == 'y')
		{
#else
		seed++;
		read_out = randomValue(seed, MAX_LENGTH);
		if( init_flag == 1)
		{
			sample = init(length);
			init_flag = 0;
			re_Size = 0;
		}

		if (re_Size == 1)
		{
#endif
			char tempbuffer[MAX_LENGTH] = {'\0'};
			old_Entries = entries(sample);

			for (uint32_t i = 0 ; i < old_Entries ; i ++)
				tempbuffer[i] = *(sample->Buffer + ((sample->Outi + i) & (old_length-1)));

			sample->Length = old_length;

			p_old_values = update_Buffer(sample);

			sample->Buffer = (char*)realloc(sample->Buffer, length * sizeof(char));

			Head = sample->Ini;
			Tail = sample->Outi;
			Entries = entries(sample);

			/**************************************/
			/**   Extension (lengh > old_length) **/
			/**************************************/

			if (length >= old_length)
			{
				if(Head <= Tail)
				{
					for( uint32_t i = Tail ; i < old_length ; i++ )
						*(sample->Buffer + (length - old_length) + i) = *(sample->Buffer + i);	// Move data from tail to the end of old size , to the new extension.

					Tail = Tail + (length - old_length);										// Move tail to the new extension but keep head in same place.
					sample->Length = length;
				}

				else
					sample->Length = length;													// If Head > Tail just update length

			}

			/**************************************/
			/**  Reduction (lengh < old_length)  **/
			/**************************************/

			else
			{
				sample->Length = length;				// This is the reduced length.

				for (uint32_t i = 0; i < length ; i++ )
					*(sample->Buffer + i) = tempbuffer[i];

				if( strlen(tempbuffer) > length)
				{
					Buffer_Full = 1;
					Tail = 0;
					Head = 0;
				}
				else
				{
					Tail = 0;
					Entries = entries(sample);
					Buffer_Full = 0;					// Check if after resize the new buffer is full
					Head = Entries;
				}
			}
			sample->Outi = Tail ;
			sample->Ini = Head ;
		}

	return 0;
  }

/** ((((((((((((((((((((((((((((((((((( init_suite() - End ))))))))))))))))))))))))))))))))))) **/

/** ################################## clean_suite() - Start ################################# **/

int clean_suite(void)

{
#if AUTO
	if (cnt != 1)
	{
#else

	if (cnt != 'y')
	{
#endif
		free(sample->Buffer);
		free(sample);

		free(p_user1->Buffer);
		free(p_user1);

		free(p_user2->Buffer);
		free(p_user2);

		free(Ascii_buffer->Buffer);
		free(Ascii_buffer);
	}

	return 0;
}

/** ################################## clean_suite() - End ################################### **/

/** {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ test_init() - Start }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} **/

void test_init(void)
{
	if (init_flag == 1)
		CU_ASSERT(NULL != init(length));
}

/** {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ test_init() - End }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} **/

/** %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test_insert() - Start %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% **/

void test_insert(void)
{
#if !AUTO

	 printf("\n\nPrevious Buffer Status:");

	 printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	 if (re_Size == 'y')
	 {
		 re_Size = 'n';
		 display( p_old_values, old_Entries, NULL ) ;		// data_out is not present when writing -
															//to the buffer, hence NULL
		 printf("\n\nPrevious Buffer after resize:\n");

		 if (old_Entries >= length)
			 Entries = length;

		 display( update_Buffer(sample), Entries, NULL ) ;
	 }
	 else
	 {
		 Entries = entries( sample);
		 display( update_Buffer(sample), Entries, NULL ) ;
	 }
	 printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

	CU_ASSERT(-1 == insert(NULL, data_in[0]));			// Error checking for NULL pointer
	uint32_t count = strlen(data_in);

	for(uint16_t i = 0; i <= count-1; i++)
	{
		uint32_t check_insert = insert(sample, data_in[i]);
		CU_ASSERT(0 == check_insert);									//PASS
		if (check_insert == -2)											//Buffer Full
		{
			printf("\n\n!!!~~> Expected buffer overflow at location: ... %d\n", \
			sample->Ini & (sample->Length-1));
			break;
		}
	}
	Entries = entries( sample);							//Update entries after intertion.


#if !AUTO
	display( update_Buffer(sample), Entries, NULL ) ;
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

}

/** %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test_insert() - End %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%**/

/******************************** test_read() - Start *************************************/

void test_read(void)
{

	CU_ASSERT(-1 == read(NULL, data_out));								// Error checking for NULL pointer

	char data_out[MAX_LENGTH] = {'\0'};
	if (read_out != 0 )
	{
		for (uint16_t i = 0; i<=read_out-1; i++)
		{
			uint32_t check_read = read(sample, data_out+i);
			CU_ASSERT(0 == check_read);									//PASS
			if(check_read == -2)
			{
				printf("\n\n!!!~~> Expected buffer empty at location: ... %d\n", \
				sample->Outi & (sample->Length-1));
				break;
			}
		}
	}
	Entries = entries (sample);

#if !AUTO
	display( update_Buffer(sample), Entries, data_out ) ;
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

}

/******************************************* test_read() - End *********************************/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ test_entries() - Start ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/

void test_entries(void)
{
	CU_ASSERT(-1 == entries(NULL));
	CU_ASSERT(-1 != entries(sample));

#if !AUTO
	printf(" \nTail: %-5d	Head: %-5d	Entries: %-d...", sample->Outi & (sample->Length-1), \
	sample->Ini & (sample->Length-1), entries( sample));
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ test_entries() - End ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~**/

/** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! test_DataValidation - Start !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**/

void test_DataValidation(void)
{
	uint32_t size1, count;
	uint8_t lastBufferFullState = Buffer_Full;		//Saving previous buffer state
	uint8_t lastBufferEmptyState = Buffer_Empty;
	int8_t check_insert, check_read;

#if !AUTO
	 printf("\n\n\t\t################################\n");
	 printf(    "\t\t#     Data validation test     #\n");
	 printf(    "\t\t################################\n\n");

	 printf("   \"This function will compare a string entered by user in a\"\n");
	 printf("   \"specified buffer,with the same data read from the buffer.\"\n\n");

	size1 = sizeValidation();						// This function checks if user input a correct value for uffer size.

	 printf("Enter a string for this buffer: ");

	 FLUSH
	 scanf("%[^\n]", data_in);

	 printf("\nYou entered: %s\n\n", data_in);

	 printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

#if AUTO
	seed++;
	size1 = randomValue (seed, 10);

	seed++;
		uint16_t strSize = randomValue(seed, MAX_LENGTH);		// Random # between 1 ~ MAX_LENGTH+1 for string size

		for( uint16_t l = 0; l <= strSize ; l++)
			{
				seed++;
				data_in[l] = randomValue(seed, 256);	// NOTE: data_in holds the generated string with random length.
			}											// Each element of data_in is a random ASCII char( 0~255) generated
														// by randomValue(seed, 256).*/
#endif

	p_user1 = init (size1);

	CU_ASSERT(-1 == insert(NULL, 'a'));					// Error checking for NULL pointer

	count = strlen(data_in);

	for(uint16_t i = 0; i <= count-1; i++)
	{
		check_insert = insert(p_user1, data_in[i]);
		CU_ASSERT(0 == check_insert);									//PASS

		if (check_insert == -2)											//Buffer Full
		{
			printf("\n\n!!!~~> Expected buffer overflow at location: ... %d\n", \
			p_user1->Ini & (p_user1->Length-1));
			break;
		}
	}

#if !AUTO
		printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif

	Entries = entries (p_user1);

	CU_ASSERT(-1 == read(NULL, data_out));								// Error checking for NULL pointer

		for ( uint16_t i = 0; i <= Entries-1; i++)						// Reading all input characters and storing into data_out
		{
			check_read = read(p_user1, data_out+i);
			CU_ASSERT(0 == check_read);									//PASS
			if ( check_read == -2 )
			{															// Check if the correct number of chars are read out.
				printf("\n\n!!!~~> Expected buffer empty at location: ... %d\n", \
				p_user1->Ini & (p_user1->Length-1));
				break;
			}
		}

	int8_t result = strncmp (data_in, data_out, Entries);				// Comparinf input data and data read from Buffer
	CU_ASSERT(0 == result);

#if !AUTO
	 if ( result != 0 )
	 {
		 printf("The data sent and recived are not equal!\n\n");
		 printf("Buffer Input:  %s\nBuffer Output: %s\n", p_user1 -> Buffer, data_out);
	 }
	 else
	 {
		 printf("The data sent and received are equal!\n\n");
		 printf("Buffer Input:  %s\nBuffer Output: %s\n", p_user1 -> Buffer, data_out);
	 }
	 printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

	 for(uint8_t k = 0 ; k < count ; k++)
	 {
		 data_in[k]  = '\0';
		 data_out[k] = '\0';
	 }
	 Buffer_Full = lastBufferFullState;			// Restoring previous buffer state
	 Buffer_Empty = lastBufferEmptyState;
}
/** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! test_DataValidation - End !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**/

/** &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& test_FibonacciSequence () - Start  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&& **/

void test_FibonacciSequence (void)
{
	uint32_t size2, i, k;
	int8_t check_insert;

	uint8_t lastBufferFullState = Buffer_Full;		//Saving previous buffer state
	uint8_t lastBufferEmptyState = Buffer_Empty;

#if !AUTO
	char InputNumbers[20];							// temp storage for two input numbers.
	uint8_t validNumber = 0;

	printf("\n\n\t\t################################\n");
	printf(    "\t\t#      Fibonacci Sequence      #\n");
	printf(    "\t\t################################\n\n");

	printf("     \"This function will take two numbers from user and calculates\"\n");
	printf("\"the fibonacci sequence, then writes the numbers into a user defined buffer.\"\n\n");

	size2 = sizeValidation();					// This function checks if user input a correct value for uffer size.
	int32_t sequence[size2];					// interger array to store calculated series.
#else
	seed++;
	size2 = randomValue (seed, 10);
	int32_t sequence[size2];					// interger array to store calculated series.

	sequence[0] = randomValue (seed, MAX_LENGTH);		// Random # between 0 ~ MAX_LENGTH for fibinacci series.
	seed++;
	sequence[1] = randomValue (seed, MAX_LENGTH);

#endif

	p_user2 = init (size2);						// Buffer p_user2 initiation
	char * userBuffer = p_user2 -> Buffer;

#if !AUTO
	printf("Please enter two integers seperated by space: ");

	 do
	 {
		 FLUSH
		 scanf("%[^\n]", InputNumbers);
		 validNumber = sscanf(InputNumbers, "%d %d", &sequence[0], &sequence[1]);
		 printf("\nYou entered: %d and %d\n\n", sequence[0], sequence[1]);
		 if  (validNumber != 2)
			 printf("\nInvalid input! please try again\n");

	 }while (validNumber != 2);
#endif

	for ( k = 0 ; k < size2 ; k++)
	{
		sequence[k+2] = sequence[k+1] + sequence[k];
		sprintf(data_out, "%d", sequence[k]);				// Using data_out[MAX_LENGTH] to keep converted numbers to chars
		if(sequence[k] < 0) break;
		strcat (data_in, " ");
		strcat( data_in, data_out);							// Appending character numbers to gether in data_in as a string
	}

#if !AUTO
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif

	CU_ASSERT(-1 == insert(NULL, 'a'));						// Error checking for NULL pointer

	for( i = 0; i <= size2-1; i++)
		{

			check_insert = insert(p_user2, data_in[i]);		// Writing the string of converted numbers to chars into the ring buffer.
			CU_ASSERT(0 == check_insert);

			if (check_insert == -2)											//Buffer Full
			{
				printf("\n\n!!!~~> Expected buffer overflow at location: ... %d\n", \
				p_user2->Ini & (size2-1));
				break;
			}
		}

		while (userBuffer[size2-1] != ' ')		// This loop eliminates incomplete numbers at the end of the list
												// when the buffer if fully loaded.
		{
			userBuffer[size2-1] = '\0';
			size2--;
		}

#if !AUTO
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("The fibonnaci sequence produced by %d and %d\n", sequence[0], sequence[1]);
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("%s\n", userBuffer);

	 for( k = 0 ; k < size2 ; k++)
	 {
		 data_in[k]  = '\0';
		 data_out[k] = '\0';
	 }
#endif

	 Buffer_Full = lastBufferFullState;			// Restoring previous buffer state
	 Buffer_Empty = lastBufferEmptyState;
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! test_FibonacciSequence - End   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ test_ASCII_Table - Start @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void test_ASCII_Table (void)
{
	uint16_t i, k;
	int8_t check_insert, check_read;
	uint8_t lastBufferFullState = Buffer_Full;		//Saving previous buffer state
	uint8_t lastBufferEmptyState = Buffer_Empty;
#if !AUTO
	printf("\n\n\t\t################################\n");
	printf(    "\t\t#          ASCII_Table         #\n");
	printf(    "\t\t################################\n\n");

	printf("     \"This function will write into the ring buffer the equivalent ascii\"\n");
	printf("        \"character of a decimal number between 0 and 255, then displays\"\n");
	printf("               \"the data read from the buffer on screen.\"\n\n");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

	Ascii_buffer = init (256);
	CU_ASSERT(-1 == insert(NULL, 'a'));						// Error checking for NULL pointer
	for( i = 0; i <= 255 ; i++)
		{
			check_insert = insert(Ascii_buffer, i);	// Writing the string of converted numbers to chars into the ring buffer.
			CU_ASSERT(0 == check_insert);
		}

	CU_ASSERT(-1 == read(NULL, data_out));							// Error checking for NULL pointer
#if !AUTO
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif
		for ( i = 0; i <= 255; i++)									// Reading all input characters and storing into data_out
		{
			check_read = read(Ascii_buffer, data_out+i);
			CU_ASSERT(0 == check_read);									//PASS
			if ( check_read == -2 )
			{															// Check if the correct number of chars are read out.
				printf("\n\n!!!~~> Expected buffer empty at location: ... %d\n", \
				Ascii_buffer->Ini & (Ascii_buffer->Length-1));
				break;
			}
#if !AUTO
			printf("%c", *(data_out + i));
			putchar('\0');
		}
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#else
		}
#endif

	for (k = 0 ; k < 256 ; k++)
		data_out[k] = '\0';

	Buffer_Full = lastBufferFullState;			// Restoring previous buffer state
	Buffer_Empty = lastBufferEmptyState;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ test_ASCII_Table - End @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ########################################### main() - Start #######################################

int main (void)
{
		CU_pSuite pSuite1 = NULL;

		// Initialize CUnit test registry

		if (CUE_SUCCESS != CU_initialize_registry())
			return CU_get_error();

		// Add suite1 to registry
#if !AUTO
		pSuite1 = CU_add_suite("Basic_Manual_Test_Suite1\n", init_suite, clean_suite);
#else
		pSuite1 = CU_add_suite("Basic_Auto_Test_Suite1\n", init_suite, clean_suite);
#endif
		if (NULL == pSuite1)
		{
			CU_cleanup_registry();
			return CU_get_error();
		}

		 // add test1 to suite1

		if ((NULL == CU_add_test(pSuite1,  " \n\n===> init() function:"    , test_init   ))||
			(NULL == CU_add_test(pSuite1,  " \n\n===> insert() function:"  , test_insert ))||
		    (NULL == CU_add_test(pSuite1,  " \n\n===> read() function:"    , test_read   ))||
		    (NULL == CU_add_test(pSuite1,  " \n\n===> entries() function:" , test_entries))||
			(NULL == CU_add_test(pSuite1,  " \n\n===> Data Validation:"    , test_DataValidation))||
			(NULL == CU_add_test(pSuite1,  " \n\n===> Fibonacci Sequence:" , test_FibonacciSequence))||
			(NULL == CU_add_test(pSuite1,  " \n\n===> ASCII Table:" 	   , test_ASCII_Table)))

		{
			CU_cleanup_registry();
			return CU_get_error();
		}

#if !AUTO
	while(cnt == 'y')
	{
#else
	int32_t loop = LOOP_COUNT;
	while(loop > 0)
	{
#endif
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();									// OUTPUT to the screen
#if !AUTO
		printf("Continue? (Y)es, (N)o ");
		FLUSH
		scanf("%c", &cnt);
		scanf("%c", &temp);
#else
		seed++;
		re_Size = randomValue(seed, 2);
		loop--;
	}
#endif
#if !AUTO
		if (cnt =='y')
		{
			printf("Do you wish to modify the buffer size? (Y)es, (N)o: ");
			scanf("%c", &re_Size);
		}

	}
#else
	printf("Auto test result for %d cycles.\n\n", LOOP_COUNT);
#endif

	CU_cleanup_registry();										// Cleaning the Registry
	return CU_get_error();
}

//########################################## main() - End #######################################
