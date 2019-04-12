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
* @file ps_rand.c
* @brief Generate a pseudo random number  
*
* This source file provides support for generating a sequence
* of pseudo random numbers given a seed value.  
*
* @author Jon Warriner
* @date February 26, 2019
* @version 1.0

*/

#include "ps_rand.h"

void ps_rand(struct randStruct *r)
{
    if(r == 0)
    {
        return;
    }

    //compute the next value in the sequence.
    //m must be a power of 2 so that we can use
    //& here instead of %
    r->X = (uint32_t)(((r->a * r->X) + r->c) & (r->m - 1));

    return;
}
