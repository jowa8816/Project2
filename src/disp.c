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
* @file disp.c
* @brief count characters and build a display
*
* This source file provides support to count characters passed in through and input ring buffer
* and builds a display of the count and puts it in an output ring buffer for transmission.
*
* @author Jon Warriner
* @date April 9, 2019
* @version 1.0

*/

#include <stdio.h>
#include <string.h>
#include "disp.h"

int32_t disp_init(disp_t *d, ring_t *ibuf, ring_t *obuf, void (*tx_func)())
{
	uint16_t i;

	//if any of the pointers are not initialized then return an error
	if((d == 0) || (ibuf == 0) || (obuf == 0))
	{
		return(-1);
	}

	//make sure the counting buffer is zero
	for(i = 0; i < 256; i++)
	{
		d->char_ctrs[i] = 0;
	}

	//make sure other control variables are zero
	d->trig = 0;
	d->i = 0;
	d->updating = 0;

	d->transmit_trig = tx_func;

	return(0);
}

void RX_task(disp_t *d)
{
	int32_t ents,i;
	uint8_t data;

	ents = entries(d->ibuf);

    //check if we have any entries in the RX buffer
	//if we do, grab the new characters and count them
    for(i = 0; i < ents; i++)
    {
    	extract(d->ibuf,(char *)&data);
    	d->char_ctrs[data]++;

    	//tell the display task that we have data to update
    	d->trig = 1;
    }
}

/**
* @brief Build the results display to send out the serial port
*
* We've got new characters and we need to update the count(s) on the display.
*
* @return void.
*/
void Display_task(disp_t *d)
{
	size_t i;
	int32_t ents;

	ents = entries(d->obuf);

	// if the TX buffer isn't empty then we haven't finished the
	// previous display update.  Let's wait and try again later.
	if(ents != 0)
	{
		return;
	}


	//RX/counting task signaled that we have new data
	//no matter where we are in the current display update,
	//start over.  We don't want to miss any characters
	if(d->trig)
	{
		d->i = 0;
		d->trig = 0;
		d->updating = 1;

		// clear the screen and print a simple header to delimit one update from the next
		sprintf(d->sbuf, "%c[2J%c[H********************\r\n", (char)27, (char)27);

		//move the string to the TX buffer
		for(i = 0; i <= strlen(d->sbuf); i++)
		{
			insert(d->obuf, d->sbuf[i]);
		}

		//kick off the transmit by enabling the interrupt
		d->transmit_trig();
	}
	//Here we are in the middle of a display update
	else if(d->updating)
	{
		//we only want to print results for characters that actually have a count
		if(d->char_ctrs[d->i])
		{
			//characters up to the space (32) are not printable
			//we'll display their hex value
			if(d->i <= 32)
			{
				sprintf(d->sbuf, "0x%x - %d\r\n", (char)d->i, d->char_ctrs[d->i]);
			}
			//characters greater than 127 are not defined
			//we'll display their hex value
			else if(d->i >= 127)
			{
				sprintf(d->sbuf, "0x%x - %d\r\n", (char)d->i, d->char_ctrs[d->i]);
			}
			else
			{
				sprintf(d->sbuf, "%c - %d\r\n", (char)d->i, d->char_ctrs[d->i]);
			}

			//move the string to the TX buffer
			for(i = 0; i <= strlen(d->sbuf); i++)
			{
				insert(d->obuf, d->sbuf[i]);
			}

			//kick off the transmit by enabling the interrupt
			d->transmit_trig();
		}
		d->i++;

		//once we've looped through all 256 possible characters
		//we'll shut down the display update task
		if(d->i == 0)
		{
			d->updating = 0;
		}
	}
}

