/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warrinerk and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file led.h
* @brief An abstraction for the LED toggle GPIO driver
*
* This header file provides an abstraction of the functions to
* implement the LED toggle driver
*
* @author Jon Warriner
* @date March 31 2019
* @version 1.0
*
*/

#ifndef LED_H_
#define LED_H_

/**
* @brief Initialize GPIO for LED toggle
*
* @return void.
*/
void LED_init();

/**
* @brief Toggle the LED
*
* @return void.
*/
void LED_toggle();

#endif /* LED_H_ */
