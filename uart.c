/*
 * File:        uart.h
 * Author:      Sam Faull
 * Description: UART peripheral functions
 */

#include <xc.h>
#include "uart.h"

/* Notes for PCB V1
 * I messed up, both UARTs are connected to pins that dont support peripheral 
 * remapping - so the following mods must be made.
 * UART1_RX = pin19 (RA0) -> pin8  (RP10/RB10)
 * UART1_TX = pin20 (RA1) -> pin14 (RP14/RB14)
 * UART2_RX = pin30 (RA2) -> pin24 (RP3/RB3)
 * UART2_TX = pin31 (RA3) -> pin23 (RP2/RB2)
 */


void initUART1(void)
{
    // PERIPHERAL PIN SELECT
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));  // Unlock the registers
    U1RX_in = RPI10;  // Assign U1Rx To Pin RP10
    RPO14 = U1TX_out;     // Assign U1Tx To Pin RP14
    __builtin_write_OSCCONL(OSCCON | (1<<6));   // Lock the registers

    U1BRG = UART1_BRGVAL; // Set baud rate
   
    U1MODEbits.STSEL = 0; // 1 Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U1MODEbits.BRGH = 0;  // Low Speed mode
    
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IFS0bits.U1RXIF = 0;    // clear Rx interrupt flag
    IFS0bits.U1TXIF = 0;    // clear Tx interrupt flag
    IEC0bits.U1TXIE = 1;    // Enable Tx Interrupt
    IEC0bits.U1RXIE = 1;    // Enable Rx interrupt
    
    U1MODEbits.UARTEN =   1;     // Enable UART
    U1STAbits.UTXEN   =   1;     // Enable UART TX
    
    /* wait at least 104 usec (1/9600) before sending first char */
    __delay_ms(120);
}

void initUART2(void)
{
    // PERIPHERAL PIN SELECT
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));  // Unlock the registers
    U2RX_in = RPI3;  // Assign U2Rx To Pin RP3
    RPO2 = U2TX_out;     // Assign U2Tx To Pin RP2
    __builtin_write_OSCCONL(OSCCON | (1<<6));   // Lock the registers
    
    U2BRG = UART2_BRGVAL; // Set baud rate
   
    U2MODEbits.STSEL = 0; // 1 Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U2MODEbits.BRGH = 0;  // Low Speed mode
    
    U2STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    U2STAbits.UTXISEL1 = 0;
    IFS1bits.U2RXIF = 0;    // clear Rx interrupt flag
    IFS1bits.U2TXIF = 0;    // clear Tx interrupt flag
    IEC1bits.U2TXIE = 1;    // Enable Tx Interrupt
    IEC1bits.U2RXIE = 1;    // Enable Rx interrupt
    
    U2MODEbits.UARTEN =   1;     // Enable UART
    U2STAbits.UTXEN   =   1;     // Enable UART TX
    
    // wait at least 104 usec (1/9600) before sending first char 
    __delay_ms(120);
}