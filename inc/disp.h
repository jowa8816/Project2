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
* @file disp.h
* @brief An abstraction for the character counting and functions
*
* This header file provides an abstraction of the functions to
* count received characters and build a display.
*
* @author Jon Warriner
* @date April 9 2019
* @version 1.0
*
*/

#ifndef DISP_H_
#define DISP_H_

#include "ring.h"

/**
* define the counter and display structured data type
*/
typedef struct
{
	void (*transmit_trig)();	//pointer to a function to trigger transmission of display data
	ring_t *ibuf;				//pointer to input ring buffer.  This contains incoming characters to be counted.
	ring_t *obuf;				//pointer to output ring buffer. This contains formatted output strings to be transmitted
	uint32_t char_ctrs[257];	//counters for each of the 256 possible characters + 1 random number
	char sbuf[80];
	uint8_t trig;				//set to 1 to trigger the display update to start or start over
	uint8_t updating;			//flag indicating that the display is currently updating
	uint16_t i;					//index of character count that is currently being updated
}disp_t;

/**
* @brief Initialize the display module
*
* @param d pointer to a display structure
* @param ibuf pointer to an input ring buffer
* @param obuf pointer to an output ring buffer
* @param tx_func pointer to a function to trigger transmission of the output buffer
*
* @return pointer to ring_t type or 0 on failure
*/
int32_t disp_init(disp_t *d, ring_t *ibuf, ring_t *obuf, void (*tx_func)());

/**
* @brief Check if there are any new characters in the RX ring buffer
*
* If there are new characters in the ring buffer then we need to add them to
* our tally
*
* @param d pointer to a display structure
*
* @return void.
*/
void RX_task(disp_t *d);

/**
* @brief Build the results display to send out the serial port
*
* We've got new characters and we need to update the count(s) on the display.
*
* @param d pointer to a display structure
*
* @return void.
*/
void Display_task(disp_t *d);

#endif /* DISP_H_ */
