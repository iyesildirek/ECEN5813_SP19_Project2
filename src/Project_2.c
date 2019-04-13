/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
* @file Project_2.c
* @brief This source file contains a c program to implement a UART driver.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 11 2019
* @version 1.4
*
*/
#if FRDM
#include "uart.h"
#include "ring.h"

#define BLOCKING 0				/* Select between blocking and non-blocking implementation */

ring_t *uart_Ring;
char buffer[] = {};
uint32_t bufferSize = 256, counter;			/* Initializing ring buffer */
typedef struct
{
	char c;
	uint32_t count;
} charCounts;

charCounts txt;

charCounts textChars[256];	// array of structures containing character and counts of it from a text



int main(void)
{
	/* Init board hardware. */
	BOARD_InitBootPins();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    uart_Ring = init(bufferSize);

#if BLOCKING
	gpio_config();
    /* Start UART for tx and rx*/
    uart_config();
    print_ASCII();
	uart_tx('\n');
#else
    __disable_irq();
    uart_config();
    gpio_config();
    __enable_irq();

#endif

	while (1)
	{

		/* This section is to count the text input chars for each character */

		if((UART0->C2 & UART_C2_RIE_MASK) == 0)		// read per RX interrupt trigger
		{
			for (int16_t i; i <= bufferSize ; i++)
				{
					insert(uart_Ring, isr_rx);
					buffer[i] = *(uart_Ring->Buffer + i);

					for (int16_t m = 0 ; m <= 255 ; m++)
						{if(buffer[i] == m)
							 counter++;
						}
						txt.c = buffer[i];
						txt.count = counter;
						textChars[i] = txt;		// adding one element to the array of structure.
				}

		}



#if BLOCKING
    	new_rx = uart_rx();
    	delay(1);
    	uart_tx(new_rx);

    	//uart_tx(test);
    	led();
#else
    	/* Blink LED */

    	led();
    	delay(20);
#endif

	}
}

/*UART0 Register Configuration per Ch12 & 39 of manual*/
void uart_config(void)
{
	/*******************************
	* Flag is set to 0 for tx mode
	* and it is set to 1 for rx mode
	******************************/
	/******************************************
	 * Clock configuration
	 *****************************************/

	SIM->SCGC4 |= 0x0400;		/* Enable UART0 clock */

	SIM->SOPT2 |= 0x04000000;	/* Select a clock source*/

	UART0->C2 = 0;				/*Disable UART tx and rx prior to modifying reg */

	/******************************************
	 * Baud Rate Configuration
	 * To set baud rate we divide selected clock
	 * by the SBR (BDH:BDL) and OSR (in C4)
	 * baud clock/((OSR+1) × BR)
	 * Base clock is 20.97 MHz / 13 / 14
	 * = 115200 Baud Rate
	 ******************************************/

	UART0->BDH = 0x00;	/*Set BDH to 0bxxx00000*/

	UART0->BDL = 0x0D;	/*Set SBR to 0bxxxx1101 or 13 decimal*/

	UART0->C4 = 0x0E;	/* Set data bit mode to 8 or 9 and over sampling ratio
	 	 	 	 	 	 * By writing 0bxxx01110 (or 14 OSR) to reg C4*/

	/******************************************
	 * Serial format Configuration.
	 * Enable and set parity to odd and 8bit data
	 *******************************************/
	UART0->C1 = 0x00;

#if BLOCKING
	UART0->C2 = 0x0C;	/* Enable both tx and rx */
#else

	/******************************
	 * Enable tx, rx, and IRQ rx
	 * by sending 0b00101100 to reg
	 ******************************/

	UART0->C2 = 0x2C;	/* Enable UART0 IRQ bit 12 */
	NVIC->ISER[0] |= 0x1000;

#endif

	/*****************************
	 * Pin MUX control
	 * Set tx port/pin to alt 3
	 *****************************/

#if BLOCKING

	/*Enable port B clock */

	SIM->SCGC5 |= 0x0400;
	PORTB->PCR[17] = 0x0300; //for tx
	PORTB->PCR[16] = 0x0300; //for rx

#else

	/*Enable port A clock*/

	SIM->SCGC5 |= 0x0200;
	PORTA->PCR[2] = 0xB0200; //for tx
	PORTA->PCR[1] = 0xB0200; //for rx

#endif
}

/******************* delay () - Start *******************/

void gpio_config(void)
{
#if BLOCKING
	/*Enable RGB - Green LED PTB19 as GPIO and output*/

	SIM->SCGC5 |= 0x400;
	PORTB->PCR[19] = 0x100;
	PTB->PDDR |= 0x80000;

#else

	/*Enable RGB - Blue LED PTD1 as GPIO and output*/

	SIM->SCGC5 |= 0x1000;
	PORTD->PCR[1] = 0x100;
	PTD->PDDR = 0x02;
	PTD->PTOR |= 0x02;

#endif
}

//############# Transmission - Start ###########
/* Hold until buffer is empty (TDRE = 1),
 * and no transmission in progress (TC = 1)*/

void tx_Status(void)
{
	while(!(UART0->S1 & 0xC0)) { }
}

/* Data transmission */

void uart_tx(uint8_t temp)
{
	tx_Status();
	UART0->D = temp; /* send a char */
}
//############# Transmission - End ###########

//{{{{{{{{{{{{{ Receiving - Start }}}}}}}}}}}}

void rx_Status(void)
{
	while(!(UART0->S1 & 0x20))	/*hold until rx buffer is full*/
	{ }
}

uint8_t uart_rx(void)	/* Data reception */
{
	rx_Status();
	uint8_t temp = UART0->D;
	return temp;
}
//{{{{{{{{{{{{{ Receiving - End }}}}}}}}}}}}}

void led(void)
{
#if BLOCKING

	PTB->PTOR |=  0x80000;	/* Toggle pin 19 Green LED */

#else

	PTD->PTOR |= 0x02;	/* Toggle pin 1 Blue LED */

#endif
}

/******************* delay () - Start *******************/

void delay (uint8_t num)	/* Delay n milliseconds @ 20.97 MHZ clock*/
 {
 	for(uint16_t i =0; i<num;i++)
 	{
 		for (uint16_t j = 0; j< 2000; j++)
 		{
 		}
 	}
 }
/******************* delay () - End *********************/

//((((((((((((( print_ASCII() - Start )))))))))))))))))))))))

void print_ASCII(void)
{
		/*### Printing the ASCII table ###*/

	for (uint16_t k=33; k <256 ; k++ )
		{
			uart_tx(k);
			delay(1);
		}
	uart_tx('\n');
	uart_tx('\r');
}
//((((((((((((( print_ASCII() - End )))))))))))))))))))))))

/*////////////////////// UART0 interrupt handler - Start////////// */

void UART0_IRQHandler(void)
{

	/******RX ISR Handler******/
	if(PORTA->ISFR |= 0x02)
		{
			isr_rx = uart_rx();
			PORTA->ISFR |= 0x02;
		}

	/******TX ISR Handler******/
	if(PORTA->ISFR |= 0x04)
	{
		uart_tx(isr_rx);
		PORTA->ISFR |= 0x04;
	}


}
/*////////////////////// UART0 interrupt handler - End////////// */
#endif