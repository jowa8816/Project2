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
* @file led.c
* @brief LED GPIO toggle driver
*
* This source file implements a GPIO driver to toggle an LED
*
* @author Jon Warriner
* @date March 31, 2019
* @version 1.0
*
*/

#include "MKL25Z4.h"
#include "led.h"

void LED_init()
{
    //need to enable the clock to PORTB before we can configure any GPIO
	//PORTB = 1 - PORTB clock enabled
    SIM->SCGC5 = SIM_SCGC5_PORTB(1);

    //We want PTB18 to be configured as a GPIO
    //MUX = 001 - PTB18 configured as ALT1 GPIO
    PORTB->PCR[18] = PORT_PCR_MUX(1);

    //We want PTB18 to be configured as an output
    GPIOB->PDDR |= (1 << 18);

}

void LED_toggle()
{
	GPIOB->PTOR = (1 << 18);
}

