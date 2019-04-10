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
//#include <string.h>
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

//counters for each of the 256 possible characters
//uint32_t char_ctrs[256] = {0};

////structure for the display update
//typedef struct
//{
//	char sbuf[80];
//	uint8_t trig;		//set to 1 to trigger the display update to start or start over
//	uint8_t updating;	//flag indicating that the display is currently updating
//	uint8_t i;			//index of character count that is currently being upated
//}disp_struct;

disp_t disp = {0};

//some test code to check how full the buffer gets
int32_t max = 0;

/**
* @brief Check if there are any new characters in the RX ring buffer
*
* If there are new characters in the ring buffer then we need to add them to
* our tally
*
* @return void.
*/
//void RX_task()
//{
//	int32_t ents,i;
//	uint8_t data;
//
//	ents = entries(rx_buf);
//
//    //check if we have any entries in the RX buffer
//	//if we do, grab the new characters and count them
//    for(i = 0; i < ents; i++)
//    {
//    	extract(rx_buf,(char *)&data);
//    	char_ctrs[data]++;
//
//    	//tell the display task that we have data to update
//    	disp.trig = 1;
//    }
//}

///**
//* @brief Build the results display to send out the serial port
//*
//* We've got new characters and we need to update the count(s) on the display.
//*
//* @return void.
//*/
//void Display_task()
//{
//	size_t i;
//	int32_t ents;
//
//	ents = entries(tx_buf);
//
//	// if the TX buffer isn't empty then we haven't finished the
//	// previous display update.  Let's wait and try again later.
//	if(ents != 0)
//	{
//		return;
//	}
//
//
//	//RX/counting task signaled that we have new data
//	//no matter where we are in the current display update,
//	//start over.  We don't want to miss any characters
//	if(disp.trig)
//	{
//		disp.i = 0;
//		disp.trig = 0;
//		disp.updating = 1;
//
//		// clear the screen and print a simple header to delimit one update from the next
//		sprintf(disp.sbuf, "%c[2J%c[H********************\r\n", (char)27, (char)27);
//
//		//move the string to the TX buffer
//		for(i = 0; i <= strlen(disp.sbuf); i++)
//		{
//			insert(tx_buf, disp.sbuf[i]);
//		}
//
//		//kick off the transmit by enabling the interrupt
//		UART_EN_TX_INT();
//	}
//	//Here we are in the middle of a display update
//	else if(disp.updating)
//	{
//		//we only want to print results for characters that actually have a count
//		if(char_ctrs[disp.i])
//		{
//			//characters up to the space (32) are not printable
//			//we'll display their hex value
//			if(disp.i <= 32)
//			{
//				sprintf(disp.sbuf, "0x%x - %d\r\n", (char)disp.i, char_ctrs[disp.i]);
//			}
//			//characters greater than 127 are not defined
//			//we'll display their hex value
//			else if(disp.i >= 127)
//			{
//				sprintf(disp.sbuf, "0x%x - %d\r\n", (char)disp.i, char_ctrs[disp.i]);
//			}
//			else
//			{
//				sprintf(disp.sbuf, "%c - %d\r\n", (char)disp.i, char_ctrs[disp.i]);
//			}
//
//			//move the string to the TX buffer
//			for(i = 0; i <= strlen(disp.sbuf); i++)
//			{
//				insert(tx_buf, disp.sbuf[i]);
//			}
//
//			//kick off the transmit by enabling the interrupt
//			UART_EN_TX_INT();
//		}
//		disp.i++;
//
//		//once we've looped through all 256 possible characters
//		//we'll shut down the display update task
//		if(disp.i == 0)
//		{
//			disp.updating = 0;
//		}
//	}
//}
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
