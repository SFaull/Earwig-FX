/*
 * File:        i2c.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_H
#define	I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "system.h"


#define Fsck	400000	// 400kHz I2C (probvably not a great idea to run this past 400kHz but seems to work!)
#define I2C_BRG	((FCY/2/Fsck)-1) 



void InitI2C1(void);
void IdleI2C1(void);
void StartI2C1(void);
void StopI2C1(void);
void WaitFlag(void);
void WaitForACK(void);
void AckI2C1(void);
void NotAckI2C1(void);
char DataRdyI2C1(void);
char MasterWriteI2C1(unsigned char data_out);
unsigned char MasterReadI2C1(void);
unsigned int MastergetsI2C1(unsigned int length, unsigned char * rdptr);
unsigned int MasterputsI2C1(unsigned char * wrptr);
void ResetVariables_I2C1(void);

#endif	/* I2C_H */