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
#include "MKL25Z4.h"
/*
 * @brief   Application entry point.
 */
int main(void) {
	char temp;

  	/* Init board hardware. */
    BOARD_InitBootClocks();

    //PORTA = 1 - PORTA clock enabled
    SIM->SCGC5 = SIM_SCGC5_PORTA(1);

    //Initialize UART0
    UART_init();

    printf("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++;
       	temp = UART_RX_block();
       	UART_TX_block(temp);
    }
    return 0 ;
}
