/*
 * File:        eeprom.c
 * Author:      Sam Faull
 * Description: Driver for 24LC256 I2C EEPROM
 */

#include <xc.h>
#include "i2c.h"
#include "eeprom.h"
#include <stdio.h>

void eeprom_writeByte(unsigned int address, unsigned char data)
{    
    ResetVariables_I2C1();
    StartI2C1();
    WaitFlag();
    
    IdleI2C1();
    
	MasterWriteI2C1(EEPROM_WRITE);			//Write Control byte
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
    MasterWriteI2C1(address >> 8);				//Write High Address
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
	
	MasterWriteI2C1(address);				//Write Low Address
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();

	MasterWriteI2C1(data);					//Write Data#
    WaitFlag();
    WaitForACK();
    
	IdleI2C1();
	StopI2C1();						//Initiate Stop Condition  
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
}

unsigned char eeprom_readByte(unsigned int address)
{    
    ResetVariables_I2C1();
    StartI2C1();
    WaitFlag();
    
    IdleI2C1();

	MasterWriteI2C1(EEPROM_WRITE);		//Write Control Byte
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
	MasterWriteI2C1(address >> 8);			//Write start address (high)
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
    MasterWriteI2C1(address);			//Write start address (low))
	WaitFlag();
    WaitForACK();

    IdleI2C1();
    
    ResetVariables_I2C1();           //send start condition
	StartI2C1();
    WaitFlag();
    
    IdleI2C1();

	MasterWriteI2C1(EEPROM_READ);	//Write control byte for read
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();

	unsigned char data = MasterReadI2C1();
    
    IdleI2C1();
    
	NotAckI2C1();				//Send NAck
    
    IdleI2C1();
	StopI2C1();						//Initiate Stop Condition
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
    
    return data;
}

void eeprom_writeSeq(unsigned int address, unsigned char *data,  unsigned char length)
{    
    ResetVariables_I2C1();
    StartI2C1();
    WaitFlag();
    
    IdleI2C1();
    
	MasterWriteI2C1(EEPROM_WRITE);			//Write Control byte
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
    MasterWriteI2C1(address >> 8);				//Write High Address
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
	
	MasterWriteI2C1(address);				//Write Low Address
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();

	MasterputslenI2C1(data, length);					//Write Data
    printf("Write: %d", *data);
    WaitFlag();
    WaitForACK();
    
	IdleI2C1();
	StopI2C1();						//Initiate Stop Condition
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
}


void eeprom_readSeq(unsigned int address, unsigned char *rdptr, unsigned char length)
{
    ResetVariables_I2C1();
    StartI2C1();
    WaitFlag();
    
    IdleI2C1();
    
	MasterWriteI2C1(EEPROM_WRITE);		//Write Control Byte
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
	MasterWriteI2C1(address >> 8);			//Write start address (high)
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();
    
    MasterWriteI2C1(address);			//Write start address (low))
	WaitFlag();
    WaitForACK();
    
    IdleI2C1();

    ResetVariables_I2C1();           //send start condition
	StartI2C1();
    WaitFlag();
    
    IdleI2C1();
    
	MasterWriteI2C1(EEPROM_READ);	//Write control byte for read
	WaitFlag();
    WaitForACK();
    
    
    IdleI2C1();

    // TODO: perform check on return value of I2C read
	MastergetsI2C1(length, rdptr);		//read Length number of bytes
    printf("Read: %d", *rdptr);
    
    IdleI2C1();
    
	NotAckI2C1();				//Send NAck
    
    IdleI2C1();
	StopI2C1();						//Initiate Stop Condition
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
}
