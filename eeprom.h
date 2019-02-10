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

#define EEPROM_PAGE_SIZE 64 // 64 byte pages
#define EEPROM_SIZE 32000   // 32k

void eeprom_erase(void);
void eeprom_writeByte(unsigned int address, unsigned char data);
unsigned char eeprom_readByte(unsigned int address);
void eeprom_writeSeq(unsigned int address, unsigned char *data);
void eeprom_readSeq(unsigned int address, unsigned char *rdptr, unsigned int length);
void nv_write(uint32_t address, uint8_t* buffer, int32_t length);

#endif	/* EEPROM_H */

