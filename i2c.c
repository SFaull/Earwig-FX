/*
 * File:   i2c.c
 * Author: Sam
 *
 * Created on 11 August 2018, 11:46
 */


#include <xc.h>
#include "i2c.h"


void InitI2C(void)
{
    I2C1CONbits.I2CEN = 1;      // I2C Enable bit
    I2C1CONbits.I2CSIDL = 0;    // Continue I2C module in Idle mode
    I2C1CONbits.SCLREL = 0;     // Hold SCL clock low (clock stretch)
    I2C1CONbits.IPMIEN = 0;     // Intelligent Peripheral Management Interface Enable bit (disabled)
    I2C1CONbits.A10M = 0;       // I2CADD is 7-bit address
    I2C1CONbits.DISSLW = 0;     // Enable Slew Rate Control for 400KHz
    I2C1CONbits.SMEN = 0;       // Disable SMBus input voltage thresholds
    I2C1CONbits.GCEN = 0;       // Disable Interrupt when General call address is received.
    I2C1CONbits.STREN = 0;      // SCL Clock Stretch Enable bit (when operating as I2C slave)
    I2C1CONbits.ACKDT = 1;      // Transmit 1 to send NACK as acknowledge
    I2C1CONbits.ACKEN = 0;      // Acknowledge sequence not in progress
    I2C1CONbits.RCEN = 0;       // Receive sequence not in progress
    I2C1CONbits.PEN = 0;        // Stop condition not in progress
    I2C1CONbits.RSEN = 0;       // Repeated Start condition not in progress
    I2C1CONbits.SEN = 0;        // Start condition not in progress
    
    I2C1BRG = I2C_BRG;
}

void IdleI2C1(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || 
          I2C1CONbits.RSEN || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);	
}

void StartI2C1(void)
{
     I2C1CONbits.SEN = 1;	/* initiate Start on SDA and SCL pins */
}

void StopI2C1(void)
{
     I2C1CONbits.PEN = 1;	/* initiate Stop on SDA and SCL pins */
}


void WaitFlag(void)
{
    while(!IFS1bits.MI2C1IF);           // wait for flag to be high
    IFS1bits.MI2C1IF=0;
}
 
void WaitForACK(void)
{
    while(I2C1STATbits.ACKSTAT);        // wait for ack receive from slave
}

char MasterWriteI2C1(unsigned char data_out)
{
    I2C1TRN = data_out;

    if(I2C1STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
    {
        while(I2C1STATbits.TRSTAT);  // wait until write cycle is complete 
        IdleI2C1();                  // ensure module is idle
        if ( I2C1STATbits.ACKSTAT ) // test for ACK condition received
    	 return ( -2 );
	    else return ( 0 );               // if WCOL bit is not set return non-negative #
    }
}
 
void ResetVariables_I2C(void)
{
    I2C1CONbits.ACKEN=0;
    I2C1CONbits.PEN=0;
    I2C1CONbits.RCEN=0;
    I2C1CONbits.RSEN=0;
    I2C1CONbits.SEN=0;
}