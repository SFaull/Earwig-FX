/*
 * File:        eeprom.h
 * Author:      Sam Faull
 * Description: 
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define EEPROM_WRITE 0xA0 
#define EEPROM_READ  0xA1

void eeprom_writeByte(unsigned int address, unsigned char data);
unsigned char eeprom_readByte(unsigned int address);
void eeprom_writeSeq(unsigned int address, unsigned char *data,  unsigned char length);
void eeprom_readSeq(unsigned int address, unsigned char *rdptr, unsigned char length);

#endif	/* EEPROM_H */

