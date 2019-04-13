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
* @file uart.h
* @brief This header file provides the uart prototypes and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 10 2019
* @version 1.1
*
*/

#define FRDM 0
#if FRDM
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/*Global variable to store input*/
uint8_t isr_rx = 0;

/*Global blocking variable to store input*/
uint8_t new_rx = 0;

//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - Start {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

void uart_config(void);
//check for transmit (tx)
void tx_Status(void);

//transmit char (tx)
//written char (tx)
void uart_tx(uint8_t temp);

//check for receive (rx)
void rx_Status (void);

//receive char (rx)
uint8_t uart_rx(void);

void gpio_config(void);

void led(void);

void delay (uint8_t num);

void print_ASCII (void);

//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - End {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

#endif
