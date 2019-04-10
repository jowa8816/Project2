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
* @file uart.h
* @brief An abstraction for the UART driver
*
* This header file provides an abstraction of the functions to
* implement the UART driver
*
* @author Jon Warriner
* @date March 31 2019
* @version 1.0
*
*/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

//#define UART_BLOCKING

/**
* @brief Initialize UART0
*
* Initialize UART0 for the application
*
* @return void.
*/
void UART_init();

/**
* @brief Is UART0 ready to transmit a character?
*
* Check the TDRE bit in S1 register to determine if the TX
* buffer is empty or has a character that has yet to go out.
*
* @return uint8_t 1 - TX buffer is empty, 0 - TX buffer is full
*/
uint8_t UART_TX_rdy();

/**
* @brief Enable UART0 TX Interrupt
*
* @return void
*/
void UART_EN_TX_INT();

/**
* @brief Disable UART0 TX Interrupt
*
* @return void
*/
void UART_DIS_TX_INT();

/**
* @brief Put a character in the UART0 TX buffer
*
* Assume the empty/full status of the buffer has already been checked
* and place a character in the TX buffer.
*
* @param data character to place in the buffer
*
* @return void
*/
void UART_TX(char data);

/**
* @brief Use UART0 to transmit a character when the buffer is empty
*
* Wait (blocking) for the UART0 TX buffer to be empty then transmit
* a character.
*
* @param data character to place in the buffer
*
* @return void
*/
void UART_TX_block(char data);

/**
* @brief Do we have a new character in the UART0 RX buffer?
*
* Check the RDRF bit in S1 register to determine if the RX
* buffer is empty or has a new character available.
*
* @return uint8_t 1 - RX buffer is full, 0 - RX buffer empty
*/
uint8_t UART_RX_full();

/**
* @brief Get a character from the UART0 RX buffer
*
* Assume the empty/full status of the buffer has already been checked
* and get a character from the RX buffer.
*
* @return character read from the buffer
*/
char UART_RX();

/**
* @brief Receive a character from UART0 when the buffer is full
*
* Wait (blocking) for the UART0 RX buffer to be full then read
* a character.
*
* @return character read from the buffer
*/
char UART_RX_block();

#endif /* UART_H_ */
