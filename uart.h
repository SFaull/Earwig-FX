/*
 * File:        uart.h
 * Author:      Sam Faull
 * Description: UART peripheral header file
 */

#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "system.h"

#define UART1_BAUDRATE    9600                          // Baud rate for UART
#define UART1_BRGVAL      ((FCY/UART1_BAUDRATE)/16)-1   // BRG value calculated from baud rate definition

#define UART2_BAUDRATE    9600                          // Baud rate for UART
#define UART2_BRGVAL      ((FCY/UART1_BAUDRATE)/16)-1   // BRG value calculated from baud rate definition


#define UART_BUFFER_SIZE		64


void uart1_init(void);
void uart2_init(void);

#endif	/* UART_H */

