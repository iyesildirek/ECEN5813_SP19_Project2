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
* @brief This header file provides the ring buffer prototypes.
*
* @author Ismail Yesildirek & Bijan Kianian
* @date March 24 2019
* @version 1.0
*
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifndef RING_H
#define RING_H

/****Function Prototypes****/

ring_t* init( int length );

/*Return 0 for success and -1 for failure*/
int insert( ring_t *ring, char data );
int rm( ring_t *ring, char *data );
/*Remove() renamed to rm() to avoid name issue*/

/* Entries should return the number of elements present in the circular buffer*/
/* The number of elements that are entered but not removed from the circular list*/
int entries( ring_t *ring );

#endif