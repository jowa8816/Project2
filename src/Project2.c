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
 
/**
 * @file    Project2.c
 * @brief   Application entry point.
 */
#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {
	char blah;

  	/* Init board hardware. */
    //BOARD_InitBootPins();
    BOARD_InitBootClocks();
    //BOARD_InitBootPeripherals();

    //UART config
    //UART0TXSRC = 00 - Selects the source for the UART0 transmit data as UART0_TX pin
    //UART0RXSRC = 0 - Selects the source for the UART0 receive data as UART0_RX pin
    //These should alread be the default settings
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

    //enable receiver and transmitter
    UART0->C2 = UART0_C2_RE(1) | UART0_C2_TE(1);

    printf("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++;
        if(UART0->S1 & UART0_S1_RDRF_MASK)
        {
        	blah = UART0->D;
        	printf("%c", blah);
        	UART0->D = blah;
        }
    }
    return 0 ;
}
