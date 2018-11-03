/*
 * File:   i2c.c
 * Author: Sam
 *
 * Created on 11 August 2018, 11:46
 */


#include <xc.h>
#include "i2c.h"


void InitI2C1(void)
{
    I2C1CONbits.I2CEN = 1;      // I2C Enable bit
    I2C1CONbits.I2CSIDL = 0;    // Continue I2C module in Idle mode
    I2C1CONbits.SCLREL = 0;     // Hold SCL clock low (clock stretch)
    I2C1CONbits.IPMIEN = 0;     // Intelligent Peripheral Management Interface Enable bit (disabled)
    I2C1CONbits.A10M = 0;       // I2CADD is 7-bit address
    I2C1CONbits.DISSLW = 1;     // Enable Slew Rate Control for 400KHz
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

void AckI2C1(void)
{
	I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 1;
}

// send a NACK
void NotAckI2C1(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
}

char DataRdyI2C1(void)
{
     return I2C1STATbits.RBF;
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

unsigned char MasterReadI2C1(void)
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN);
    I2C1STATbits.I2COV = 0;
    return(I2C1RCV);
}

unsigned int MastergetsI2C1(unsigned int length, unsigned char * rdptr)
{
    unsigned int i2c1_data_wait = 1000;  // this was an argument, but lets hardcode it in here
    int wait = 0;
    while(length)                    /* Receive length bytes */
    {
        I2C1CONbits.RCEN = 1;
        while(!DataRdyI2C1())
        {
            if(wait < i2c1_data_wait)
                wait++ ;                 
            else
            return(length);          /* Time out, 
                                        return number of byte/word to be read */
        }
        wait = 0;
        *rdptr = I2C1RCV;            /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C1CONbits.ACKDT = 1;
            I2C1CONbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C1CONbits.ACKDT = 0;
            I2C1CONbits.ACKEN = 1;
        }
            while(I2C1CONbits.ACKEN == 1);    /* Wait till ACK/NACK sequence 
                                                 is over */
    }
    /* return status that number of bytes specified by length was received */
    return 0;
}

unsigned int MasterputsI2C1(unsigned char * wrptr)
{
    while(*wrptr)                           //transmit data until null char
    {
        if(MasterWriteI2C1(*wrptr) == -1)	    // write a byte
        return -3;                          //return with write collison error

        while(I2C1STATbits.TBF);             //Wait till data is transmitted.

        IdleI2C1();
        wrptr++;
    }
    return 0;			
}

unsigned int MasterputslenI2C1(unsigned char * wrptr, unsigned char length)
{
    while(length)                           //transmit data until null char
    {
        if(MasterWriteI2C1(*wrptr) == -1)	    // write a byte
        return -3;                          //return with write collison error

        while(I2C1STATbits.TBF);             //Wait till data is transmitted.

        IdleI2C1();
        wrptr++;
        length--;
    }
    return 0;			
}
 
void ResetVariables_I2C1(void)
{
    I2C1CONbits.ACKEN=0;
    I2C1CONbits.PEN=0;
    I2C1CONbits.RCEN=0;
    I2C1CONbits.RSEN=0;
    I2C1CONbits.SEN=0;
}