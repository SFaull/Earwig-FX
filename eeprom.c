/*
 * File:        eeprom.c
 * Author:      Sam Faull
 * Description: Driver for 24LC256 I2C EEPROM
 */

#include <xc.h>
#include "i2c.h"
#include "eeprom.h"
#include <stdio.h>

void eeprom_erase(void)
{
    int i;
    for (i=0; i<EEPROM_SIZE; i++)
    {
        eeprom_writeByte(i, 0xFF);
    }
}

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
    
    __delay_ms(10);  // TODO FIXME - must allow at least this long before attempting to read else will hang
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

// this only work up to a page
void eeprom_writeSeq(unsigned int address, unsigned char *data)
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

	MasterputsI2C1(data);					//Write Data
    WaitFlag();
    WaitForACK();
    
	IdleI2C1();
	StopI2C1();						//Initiate Stop Condition
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
    __delay_ms(1500);  // TODO FIXME - must allow at least this long before attempting to read else will hang
}



void nv_write(uint32_t address, uint8_t* buffer, int32_t length)
{
	int32_t bytesRemaining = length;
	uint8_t *bufPos = buffer;
	uint32_t eePos = address;
	int32_t result = 0;
	int32_t clearToWrite;
		
	while(bytesRemaining)
	{
		/*Find the maximum number of bytes that can be written in one go, from the current location*/
		clearToWrite = EEPROM_PAGE_SIZE - (eePos % EEPROM_PAGE_SIZE);

		/*Make sure we don't overshoot at the end*/
		if(clearToWrite > bytesRemaining)
		{
			clearToWrite = bytesRemaining;
		}
        
        
        
        
        
        
        
            ResetVariables_I2C1();
            StartI2C1();
            WaitFlag();

            IdleI2C1();

            MasterWriteI2C1(EEPROM_WRITE);			//Write Control byte
            WaitFlag();
            WaitForACK();

            IdleI2C1();

            MasterWriteI2C1(eePos >> 8);				//Write High Address
            WaitFlag();
            WaitForACK();

            IdleI2C1();

            MasterWriteI2C1(eePos);				//Write Low Address
            WaitFlag();
            WaitForACK();

            IdleI2C1();

            MasterputslenI2C1(bufPos, clearToWrite);					//Write Data
            WaitFlag();
            WaitForACK();

            IdleI2C1();
            StopI2C1();						//Initiate Stop Condition

            WaitFlag();
            WaitForACK();
            IdleI2C1();	//Wait to complete  
        
            __delay_ms(15);  // TODO FIXME - must allow at least this long before attempting to read else will hang
        

        bytesRemaining -= clearToWrite;
        bufPos += clearToWrite;
        eePos += clearToWrite;
        result += clearToWrite;
	}
	
}



void eeprom_readSeq(unsigned int address, unsigned char *rdptr, unsigned int length)
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
    
    IdleI2C1();
    
	NotAckI2C1();				//Send NAck
    
    IdleI2C1();
	StopI2C1();						//Initiate Stop Condition
    
    WaitFlag();
    WaitForACK();
    IdleI2C1();	//Wait to complete  
}
