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
* @file uart.c
* @brief This source file contains a c program to implement a uart driver.
*
* @authors: Ismail Yesildirek, Bijan Kianian
* @date April 6 2019
* @version 1.0
*
*/

#include "uart.h"

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    //BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    uart_config();
    gpio_config();
    char test = 'A';
    while (1)
    {
    	uart_tx(test);
    	delay(1);
    }
}

/*UART0 Register Configuration per Ch12 & 39 of manual*/
void uart_config(void) {
/******************************************
 * Clock configuration
 *****************************************/
/* Enable UART0 clock */
SIM->SCGC4 |= 0x0400;
/* Select a clock source*/
SIM->SOPT2 |= 0x04000000;
/*Disable UART tx and rx prior to modifying registers */
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
/* Serial format Configuration
 * Enable and set parity to odd and 8bit data
 *******************************************/
UART0->C1 = 0x00;
/*Enable transmit*/
UART0->C2 = 0x08;
//UART0->C2 = 0x04; // rx enable
/*****************************
 * Pin MUX control
 * Set tx port/pin to alt 3
 *****************************/
/*Enable port B clock */
SIM->SCGC5 |= 0x0400;
PORTB->PCR[17] = 0x0300; //tx
//PORTB->PCR[16] = 0x0300; //for rx
}

    void tx_Status(void)
    {
 	   /* hold until buffer is empty b7 of reg*/
 	   while(!(UART0->S1 & 0x80));
    }

   void uart_tx(char temp)
   {
	   tx_Status();
	   UART0->D = temp;
	   led();
	   /*hold until tx is done b6 of reg*/
	   while(!(UART0->S1 & 0x40));
		   
   }

   char uart_rx(void)
   {
	   
   }
    void gpio_config(void)
    {
    	/*Enable RGB - Green LED PTB19 as GPIO and output*/
    	PORTB->PCR[19] = 0x100;
    	PTB->PDDR |= 0x80000;
    }

    void led()
    {
    	/* Toggle Green LED */
    	PTB->PTOR |=  0x80000; 
    }

    void delay (int num)
    {
    	for(int i =0; i<num*1000;i++)
    	{
    		for (int j = 0; j< 1000; j++)
    		{
    		}
    	}
    }
