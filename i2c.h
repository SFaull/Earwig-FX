/*
 * File:        i2c.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#include <xc.h> // include processor files - each processor file is guarded.  
#include "system.h"


#define Fsck	100000	// 100kHz I2C 
#define I2C_BRG	((FCY/2/Fsck)-1) 


void i2c_init();
void WaitFlag(void); 
void WaitForACK(void); 
void ResetVariables_I2C(void);


