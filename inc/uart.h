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
* @brief This header file provides the uart prototypes, and include files.
*
* @authors: Ismail Yesildirek & Bijan Kianian
* @date April 6 2019
* @version 1.0
*
*/

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//#define

//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - Start {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

void uart_config(void);
//check for transmit (tx)
void tx_Status(void);

//transmit char (tx)
//written char (tx)
void uart_tx(unsigned char temp[]);

//check for receive (rx)
void rx_Status (void);

//receive char (rx)
void uart_rx(void);

//written char (rx)

void gpio_config(void);
void led(void);

void delay (int num);
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} Functions Prototype - End {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{

