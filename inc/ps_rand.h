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
* @file ps_rand.h
* @brief An abstraction for the ps_rand function
*
* This header file provides an abstraction of the function to
* generate a pseudo random number sequence.
*
* @author Jon Warriner
* @date February 26 2019
* @version 1.0
*
*/
#ifndef __PS_RAND_H__
#define __PS_RAND_H__

#include <stdio.h>
#include <stdint.h>

// define the constants that we'll use in our rand function
//'m' is the modulus.  This pseudo random number generator will
//produce integers in the range of 0 to m-1.  For this implementation
//'m' must be a power of 2.
#define RAND_M	0x0000000100000000		//2^32
//'a' is the multiplier.  The previous output will be multiplied by 'a.'  
//The valid range for 'a' is 0<a<m
#define RAND_A	1664525
//'c' is the increment.  This is added to the previous output after it
//is multiplied by 'a.'  The valid range for 'c' is 0<=c<m
#define RAND_C	1013904223

//Hull-Dobell Theorem
//A full period of 'm' can be achieved for all seed values if and only if
//
//1) m and c are relatively prime. (The only common factor of the two is 1.)
//2) a-1 is divisible by all prime factors off m.
//3) a-1 is divisible by 4 if m is divisible by 4.  

//define a structure for a block of allocated memory
struct randStruct
{
    uint64_t a;
    uint64_t c;
    uint64_t m;
    uint32_t X;
};


/**
* @brief Generate a sequence of pseudo random numbers
*
* Given a pointer to a char command buffer, this will display
* the contents of a block of previously allocated memory.
*
* @param cmd Pointer to a command buffer
* @param b   Pointer to a memory block structure
*
* @return void.
*/
void ps_rand(struct randStruct *r);
#endif /* __PS_RAND_H__ */

