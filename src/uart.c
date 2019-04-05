/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warriner and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file uart.c
* @brief UART driver
*
* This source file implements a UART driver
*
* @author Jon Warriner
* @date March 31, 2019
* @version 1.0
*
*/

#include "MKL25Z4.h"
#include "uart.h"

void UART_init()
{
    //UART config
    //UART0TXSRC = 00 - Selects the source for the UART0 transmit data as UART0_TX pin
    //UART0RXSRC = 0 - Selects the source for the UART0 receive data as UART0_RX pin
    //These should already be the default settings
    //We want all other bit in this register to be 0 too
    SIM->SOPT5 = 0;

    //UART0SRC = 1 - MCGFLLCLK clock is the source for the UART0 transmit and receive clock
    //The rest of the bits can be 0
    SIM->SOPT2 = SIM_SOPT2_UART0SRC(1);

    //UART0 = 1 - UART0 clock enabled
    //Don't mess with other bits
    SIM->SCGC4 |= SIM_SCGC4_UART0(1);

    //PORTA = 1 - PORTA clock enabled
    SIM->SCGC5 = SIM_SCGC5_PORTA(1);

    //Configure UART pins
    //MUX = 010 - PTA1 configured as ALT2 UART0_RX
    PORTA->PCR[1] = PORT_PCR_MUX(2);
    //MUX = 010 - PTA2 configured as ALT2 UART0_TX
    PORTA->PCR[2] = PORT_PCR_MUX(2);

    //Configure baudrate to 115200
    //47972352/(115200 * (15+1)) = 26.03
    //BR = 26 (0x1A)
    UART0->BDL = UART0_BDL_SBR(0x1A);
    //OSR = 0xF (should alread be power-up default)
    UART0->C4 = UART0_C4_OSR(0xF);

#ifndef UART_BLOCKING
    //set UART0 interrupt priority to 0
#define UART_PRI	0
    NVIC->IP[_IP_IDX(UART0_IRQn)]  = ((uint32_t)(NVIC->IP[_IP_IDX(UART0_IRQn)]  & ~(0xFFUL << _BIT_SHIFT(UART0_IRQn))) |
       (((UART_PRI << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(UART0_IRQn)));

    //enable the UART IRQ
    NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)UART0_IRQn) & 0x1FUL));

    UART0->C2 |= UART0_C2_RIE(1);
#endif

    //enable receiver and transmitter
    UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

}

uint8_t UART_TX_rdy()
{
	return((UART0->S1 & UART_S1_TDRE_MASK) >> UART_S1_TDRE_SHIFT);
}

void UART_TX(char data)
{
	UART0->D = data;
}

void UART_TX_block(char data)
{
	//just wait here until the TX buffer is empty
	while(UART_TX_rdy() == 0);

	//Now we can spit out the character
	UART_TX(data);
}

uint8_t UART_RX_full()
{
	return((UART0->S1 & UART_S1_RDRF_MASK) >> UART_S1_RDRF_SHIFT);
}

char UART_RX()
{
	return(UART0->D);
}

char UART_RX_block()
{
	//just wait here until the RX buffer is full
	while(UART_RX_full() == 0);

	//Now we can grab the character
	return(UART_RX());
}

