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


#define Fsck	100000	// 100kHz I2C 
#define I2C_BRG	((FCY/2/Fsck)-1) 


void InitI2C(void);
void IdleI2C1(void);
void StartI2C1(void);
void StopI2C1(void);
void WaitFlag(void);
void WaitForACK(void);
char MasterWriteI2C1(unsigned char data_out);
void ResetVariables_I2C(void);

#endif	/* I2C_H */