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
* @file unittest_long.c
* @brief Ring buffer long, random unit testing  
*
* This source file provides support for unit testing a ring buffer.
* The basis of this file was given as part of the ring buffer
* project for ECEN 5813.   
*
* @author Jon Warriner
* @date March 14, 2019
* @version 1.0

*/

#include "ring.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>

//#define DEBUG		//uncomment this to display detailed buffer info for each step of the tests
#define TEST_LENGTH 64	//Buffer length must be a power of 2. 
#define TEST_DURATION 1000000 

ring_t *ring = 0;
ring_t *ring2 = 0;

uint32_t g_inserts = 0;
uint32_t f_inserts = 0;
uint32_t g_extracts = 0;
uint32_t f_extracts = 0;

#ifdef DEBUG
// Debug function to display the ring buffer parameters and contents
void dispBuf(ring_t *r)
{
int32_t j;

    printf("\r\nLength: %d\r\n", r->Length);
    printf("Ini: %d\r\n", r->Ini);
    printf("Outi: %d\r\n", r->Outi);
    printf("Buffer: \r\n");
    for(j = 0; j < TEST_LENGTH; j++)
    {
        printf("%d\r\n", r->Buffer[j]);
    }
    printf("Entries: %d\r\n\r\n", entries(r));
}
#endif

/* The suite initialization function.
 * Nothing to do in here for now.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
    return 0;
}

/* The suite cleanup function.
 * Nothing to do in here for now.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}

/* Simple test of init().
 * Attempt an invalid and then valid initialization of a ring buffer.
 */
void testLONG(void)
{
    int32_t i;
    int32_t ents;
    int32_t r;
    int32_t err;
    char wdata = 5;
    char rdata = 0;
    char rdata2 = 0;
	
    // Create a buffer
    // check that we got a non-zero pointer
    ring = ring_init(TEST_LENGTH);
    CU_ASSERT(0 != ring);

    // Create a second buffer
    // this one is intended to have the same data as the first
    // we'll use this to compare and check
    ring2 = ring_init(TEST_LENGTH);
    CU_ASSERT(0 != ring2);
	
    srand(time(0));
	
    for(i = 0; i < TEST_DURATION; i++)
    {
        //get the number of current entries in the buffer
        //we will use this to determine whether or not to expect
        //an error when we insert or extract to/from the buffer
        ents = entries(ring);
		
        r = rand();
		
        //if r is odd, insert a character into the buffer
        //if r is even, extract a character from the buffer
        if(r & 0x1)
        {
            wdata = (char)(rand() & 0x00FF);
            err = insert(ring, wdata);
			
            //if the buffer is full we should expect a non-zero error
            if(ents < TEST_LENGTH)
            {
                CU_ASSERT(err == 0);
                // assume everything went ok and insert data into the second buffer as well
                err = insert(ring2, wdata);
                g_inserts++;
            }
            else
            {
                CU_ASSERT(err != 0);
                f_inserts++;
            }
        }
        else
        {
            err = extract(ring, &rdata);
			
            //if the buffer is empty we should expect a non-zero error
            if(ents != 0)
            {
                CU_ASSERT(err == 0);
                //assume the extract went ok and extract from the second buffer as well.  
                //the dat from the first and second buffers should match
                err = extract(ring2, &rdata2);
                CU_ASSERT(rdata == rdata2);
                g_extracts++;
            }
            else
            {
                CU_ASSERT(err != 0);
                f_extracts++;
            }
        }
		
    }

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST init() THEN insert() THEN remove() THEN insert() with wrap */
   /* finally remove() with wrap.  */
   if ((NULL == CU_add_test(pSuite, "Long running random test", testLONG)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();

   //print out some of my own statistics
   printf("%d successful insertions\r\n", g_inserts);
   printf("%d failed insertions\r\n", f_inserts);
   printf("%d successful extractions\r\n", g_extracts);
   printf("%d failed extractions\r\n", f_extracts);
   printf("total = %d\r\n", g_inserts + f_inserts + g_extracts + f_extracts);
   return CU_get_error();
}

