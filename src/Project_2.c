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
* @date April 10 2019
* @version 1.3
*
*/

#include "uart.h"

/* Select between blocking and non-blocking implementation */
//#define BLOCKING 1

int main (void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    //BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
//    signed char new_rx = ' ';

#if BLOCKING
    /* Start UART for tx and rx*/
    uart_config();
    gpio_config();
#else
    __disable_irq();
    uart_config();
    gpio_config();
    __enable_irq();

#endif

    //unsigned char myString[] = "Hello!?";
    //signed char test = '\n';
    uint16_t test = '\n';
    //unsigned char *rx = "test it!\n";

	while (1)
	{
		/* 8 bit data loop*/
/* 	   for (int i = 0; i < 7; i++) {
 	   }
    	uart_tx(&test);
    	delay(1);
*/
#if BLOCKING
    	new_rx = uart_rx();
    	uart_tx(new_rx);
    	uart_tx(test);
    	led();
    	delay(1);
#else
    	/* Toggle LED */
    	led();
    	delay(1);
#endif
	}
}

/*UART0 Register Configuration per Ch12 & 39 of manual*/
void uart_config(void) {
	/*******************************
	* Flag is set to 0 for tx mode
	* and it is set to 1 for rx mode
	******************************/
	/******************************************
	 * Clock configuration
	 *****************************************/
	/* Enable UART0 clock */
	SIM->SCGC4 |= 0x0400;
	/* Select a clock source*/
	SIM->SOPT2 |= 0x04000000;
	/*Disable UART tx and rx prior to modifying reg */
	UART0->C2 = 0;
	/******************************************
	 * Baud Rate Configuration
	 * To set baud rate we divide selected clock
	 * by the SBR (BDH:BDL) and OSR (in C4)
	 * baud clock/((OSR+1) × BR)
	 * Base clock is 20.97 MHz / 13 / 14
	 * = 115200 Baud Rate
	 ******************************************/
	/*Set BDH to 0bxxx00000*/
	UART0->BDH = 0x00;
	/*Set SBR to 0bxxxx1101 or 13 decimal*/
	UART0->BDL = 0x0D;
	/* Set data bit mode to 8 or 9 and over sampling ratio
	 * By writing 0bxxx01110 (or 14 OSR) to reg C4*/
	UART0->C4 = 0x0E;
	/******************************************
	/* Serial format Configuration.
	 * Enable and set parity to odd and 8bit data
	 *******************************************/
	UART0->C1 = 0x02;
	#if BLOCKING
	/* Enable both tx and rx */
	UART0->C2 = 0x0C;
	#else
	/******************************
	 * Enable tx, rx, and IRQ rx
	 * by sending 0b00101100 to reg
	 ******************************/
	UART0->C2 = 0x2C;
	/* Enable UART0 IRQ bit 12 */
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

	/* Set pins to alt 2 for uart and IRQ */
	PORTA->PCR[2] = 0xB0200; //for tx
	PORTA->PCR[1] = 0xB0200; //for rx
	#endif
}

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

    void tx_Status(void)
    {
 	   /* hold until buffer is empty b7 of reg*/
 	   while(!(UART0->S1 & 0x80));
    }

   void uart_tx(uint16_t temp)
   {
	   tx_Status();
	   UART0->D = temp;
	   /*hold until tx is done b6 of reg*/
	   while(!(UART0->S1 & 0x40))
	   {}
   }

   void rx_Status (void)
   {
	   /*hold until rx buffer is full*/
	   	   while(!(UART0->S1 & 0x20))
	   	   {}
   }

   uint16_t uart_rx(void)
   {
	  rx_Status();
	  uint16_t temp;
	  temp = UART0->D;
	 // uart_tx(&temp);
	  return temp;
   }

void led(void)
{
#if BLOCKING
	/* Toggle pin 19 Green LED */
	PTB->PTOR |=  0x80000;
#else
	/* Toggle pin 1 Blue LED */
	PTD->PTOR |= 0x02;
#endif
}

void delay (uint8_t num)
{
	for(int i =0; i<num*1000;i++)
	{
		for (int j = 0; j< 1000; j++)
		{
		}
	}
}

//((((((((((((( print_ASCII() - Start )))))))))))))))))))))))

void print_ASCII(void)
{
		/*### Printing the ASCII table ###*/
	for (uint16_t k=0; k <256 ; k++ )
		{
			uart_tx(k);
			delay(30);
		}
	uart_tx('\n');
	uart_tx('\r');
}
//((((((((((((( print_ASCII() - End )))))))))))))))))))))))

/* UART0 interrupt handler */
void UART0_IRQHandler(void)
{
	/******TX ISR Handler******/
	if(PORTA->ISFR |= 0x04)
	{
		uart_tx(isr_rx);
		PORTA->ISFR |= 0x04;
	}

	/******RX ISR Handler******/
	if(PORTA->ISFR |= 0x02)
		{
		isr_rx = uart_rx();
		PORTA->ISFR |= 0x02;
			}
}
