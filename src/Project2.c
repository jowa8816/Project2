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
#include "disp.h"
#include "MKL25Z4.h"

//#define UART_TESTING
#define APPLICATION

#if !defined(UART_BLOCKING) && defined(UART_TESTING)
#define TXRX_BUF_SIZE	16
ring_t *txrx_buf = 0;
//some test code to check how full the buffer gets
int32_t max = 0;
#elif defined(APPLICATION)
#define RX_BUF_SIZE	16
#define TX_BUF_SIZE	32

ring_t *rx_buf = 0;
ring_t *tx_buf = 0;

disp_t disp = {0};

//some test code to check how full the buffer gets
int32_t max = 0;
#endif

/*
 * @brief   Application entry point.
 */
int main(void) {
#if defined(UART_BLOCKING) && defined(UART_TESTING)
	char temp;
#endif

  	/* Init board hardware. */
    BOARD_InitBootClocks();

#if !defined(UART_BLOCKING) && defined(UART_TESTING)
    //Inialize the GPIO for LED blinking
    LED_init();

    txrx_buf = ring_init(TXRX_BUF_SIZE);
#elif defined(APPLICATION)
    rx_buf = ring_init(RX_BUF_SIZE);
    tx_buf = ring_init(TX_BUF_SIZE);
#endif

    //Initialize UART0
    UART_init();

    //Initialize the count and display module
    disp_init(&disp, rx_buf, tx_buf, &UART_EN_TX_INT);

    printf("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++;
#if defined(UART_BLOCKING) && defined(UART_testing)
        temp = UART_RX_block();
       	UART_TX_block(temp);
#elif !defined(UART_BLOCKING) && defined(UART_TESTING)
        LED_toggle();
#elif defined(APPLICATION)
        RX_task(&disp);
        Display_task(&disp);
#endif
    }
    return 0 ;
}

#if !defined(UART_BLOCKING) && defined(UART_TESTING)
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
#elif defined(APPLICATION)
void UART0_DriverIRQHandler(void)
{
char temp;

	//if the UART has a character available, grab it and put it in the ring buffer
	if(UART_RX_full())
	{
		insert(rx_buf, UART_RX());
	}

	//just some test code to see how full the buffer gets
	if(entries(rx_buf) > max)
	{
		max = entries(rx_buf);
	}

	//if the UART is ready to transmit, and we still have data in the buffer, then grab the next character and transmit it.
	if(UART_TX_rdy())
	{
		if(entries(tx_buf) != 0)
		{
			extract(tx_buf, &temp);
			UART_TX(temp);
		}
		else
		{
			UART_DIS_TX_INT();
		}
	}
}
#endif
