/*
 * File:   i2c.c
 * Author: Sam
 *
 * Created on 11 August 2018, 11:46
 */


#include <xc.h>
#include <i2c.h>
#include <stdio.h>
#include <string.h> /* memset */
#include <stdbool.h>
#include <stdlib.h>
#include "i2c.h"


void i2c_init()
{
    unsigned int config;
    
     config = (
              I2C1_ON           // I2C Enable bit
            & I2C1_IDLE_CON     // Continue I2C module in Idle mode
            & I2C1_CLK_HLD      // Hold SCL clock low (clock stretch)
            & I2C1_IPMI_DIS     // Intelligent Peripheral Management Interface Enable bit
            & I2C1_7BIT_ADD     // I2CADD is 7-bit address
            & I2C1_SLW_EN       // Enable Slew Rate Control for 400KHz
            & I2C1_SM_DIS       // Disable SMBus input voltage thresholds
            & I2C1_GCALL_DIS    // Disable Interrupt when General call address is received.
            & I2C1_STR_DIS      // SCL Clock Stretch Enable bit (when operating as I2C slave)
            & I2C1_NACK         // Transmit 1 to send NACK as acknowledge
            & I2C1_ACK_DIS      // Acknowledge sequence not in progress
            & I2C1_RCV_DIS      // Receive sequence not in progress
            & I2C1_STOP_DIS     // Stop condition not in progress
            & I2C1_RESTART_DIS  // Repeated Start condition not in progress
            & I2C1_START_DIS    // Start condition not in progress
            );
    OpenI2C1(config, I2C_BRG);          //I2CBRG_VAL = 0x78
    
    IdleI2C1();                 // Wait until I2C Bus is Inactive
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
 
void ResetVariables_I2C(void)
{
    I2C1CONbits.ACKEN=0;
    I2C1CONbits.PEN=0;
    I2C1CONbits.RCEN=0;
    I2C1CONbits.RSEN=0;
    I2C1CONbits.SEN=0;
}