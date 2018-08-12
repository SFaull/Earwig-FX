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

void eeprom_write_byte(unsigned int address, unsigned char data);
unsigned char eeprom_read_byte(unsigned int address);
void eeprom_write_seq(unsigned int address, unsigned char *data);
void eeprom_read_seq(unsigned int address, unsigned char *rdptr, unsigned char length);

#endif	/* EEPROM_H */

