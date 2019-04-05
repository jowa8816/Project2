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
* @file Project2.c
* @brief main file for the memory test project.
*
* This source file provides the main() function for Project2
*
* @author Jon Warriner
* @date March 28, 2019
* @version 1.0
*
*/
 
#include <stdio.h>
#include "clock_config.h"
#include "uart.h"
#include "led.h"
#include "ring.h"
#include "MKL25Z4.h"

#ifndef UART_BLOCKING
#define TXRX_BUF_SIZE	16
ring_t *txrx_buf = 0;
//some test code to check how full the buffer gets
int32_t max = 0;
#endif

/*
 * @brief   Application entry point.
 */
int main(void) {
#ifdef UART_BLOCKING
	char temp;
#endif

  	/* Init board hardware. */
    BOARD_InitBootClocks();

//    //PORTA = 1 - PORTA clock enabled
//    SIM->SCGC5 = SIM_SCGC5_PORTA(1);

    //Initialize UART0
    UART_init();

    //Inialize the GPIO for LED blinking
    LED_init();

#ifndef UART_BLOCKING
    txrx_buf = ring_init(TXRX_BUF_SIZE);
#endif

    printf("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++;
#ifdef UART_BLOCKING
        temp = UART_RX_block();
       	UART_TX_block(temp);
#else
        LED_toggle();
#endif
    }
    return 0 ;
}

#ifndef UART_BLOCKING
void UART0_DriverIRQHandler(void)
{
char temp;

	//if the UART has a character available, grab it and put it in the ring buffer
	if(UART_RX_full())
	{
		insert(txrx_buf, UART_RX());
	}

	//just some test code to see how full the buffer gets
	if(entries(txrx_buf) > max)
	{
		max = entries(txrx_buf);
	}

	//if the UART is ready to transmit, and we still have data in the buffer, then grab the next character and transmit it.
	if(UART_TX_rdy() && (entries(txrx_buf) != 0))
	{
		extract(txrx_buf, &temp);
		UART_TX(temp);
	}
}
#endif
