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
* @file ring.h
* @brief This header file provides the ring buffer prototypes, and buffer structure.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 6 2019
* @version 1.4
*
*/

#define FRDM 0
#ifndef RING_H
#define RING_H

#if !FRDM

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define FLUSH 	fflush(stdin); scanf("%c", &temp);		// Flushing key board buffer
#define MAX_LENGTH 1024
#define LOOP_COUNT 5
uint8_t Buffer_Full , Buffer_Empty ;	// A flag to indicate the buffer state.

typedef char uint_8;

typedef struct
{
	char *Buffer;
	uint32_t Length;
	uint32_t Ini;
	uint32_t Outi;
} ring_t;


//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - Start {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

ring_t *init (uint32_t);
int8_t insert (ring_t*, char);
int8_t read (ring_t *, char*);
int32_t entries (ring_t*);
uint8_t Power_Of_Two (uint32_t);
void display (ring_t*, int32_t, char*);
ring_t *update_Buffer ( ring_t* );
uint32_t sizeValidation (void);
uint16_t randomValue ( uint32_t , uint16_t );

//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - End {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

#endif
