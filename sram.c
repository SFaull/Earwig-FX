/*
 * File:        sram.h
 * Author:      Sam Faull
 * Description: Driver for 23LC1024 SRAM
 */

#include <xc.h>
#include <stdbool.h>
#include <stdio.h>
#include "sram.h"
#include "system.h"

/*
 * SPI connections:
 * SCK = pin37 (RP20)
 * SDO = pin36 (RP19)
 * SDI = pin33 (RP4)
 * CS0 = pin32 (RA8)
 * CS1 = pin35 (RA9)
 */

static unsigned char spi_command(unsigned char data);
static void initSPI2(void);

static void initSPI2(void)
{ 
    // PERIPHERAL PIN SELECT
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));  // Unlock the registers    
    RPO19 = SDO2_out; //RP19 (pin 36) = SDO2
    RPO20 = SCK2_out; //RP20 (pin 27) = SCK2
    SDI2_in = RPI4; //RP4 (pin 33) = SDI2
    __builtin_write_OSCCONL(OSCCON | (1<<6));   //Lock the registers
    
    TRISAbits.TRISA8 = 0; //RA8 (Pin 32) = output = sram_SS #1 (SPI slave select)
    TRISAbits.TRISA9 = 0; //RA9 (Pin 35) = output = sram_SS #2 (SPI slave select)

    SRAM0_SS = 1;                //disable sram #1
    SRAM1_SS = 1;
    
    SPI2CON1bits.DISSCK = 0;    //clock enabled (Internal serial clock is enabled)
    SPI2CON1bits.DISSDO = 0;    //SDO enabled (SDOx pin is controlled by the module)
    SPI2CON1bits.MODE16 = 0;    //8 bit mode (set to 1 for 16 bit)
    SPI2CON1bits.CKE = 0;       //data changes on rising edge (idle to active)
    SPI2CON1bits.CKP = 0;       //idle is low
    SPI2CON1bits.PPRE = 3;      //1:1 primary prescaler
    SPI2CON1bits.SPRE = 6;      //2:1 secondary prescaler (20Mhz SPI clock)
    SPI2CON1bits.MSTEN = 1;     //master mode
    SPI2CON1bits.SMP = 0;       //data sampled in middle (Input data is sampled at the middle of data output time)
    SPI2STATbits.SPIEN = 1;     //enable SPI      
}

static unsigned char spi_command(unsigned char data)
{
    SPI2BUF = data;                 // Put data into buffer
    while (!SPI2STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    return SPI2BUF;     // return whatever has been put in the receive buffer
}

void sram_fill(unsigned char sramdata) 
{
    unsigned char temp;
    unsigned long a;
    
    SRAM0_SS = 0;                    // Enable SRAM

    temp = spi_command(WRITE);      // Send WRITE command, to be followed by 24-bit address

    for (a=0; a<3; a++)             // send 24 bit address in 3 bytes
        temp = spi_command(0);
    
    for (a=0; a<SRAM_SIZE; a++)            // write byte to entire of memory
        temp = spi_command(sramdata);

    SRAM0_SS = 1;                          // Disable SRAM
}

void sram_init(unsigned char sram_mode)
{
    initSPI2(); // init spi
    
    // init chip 0
    SRAM0_SS = 0; 
    spi_command(WRMR);   // Put into WRITE mode
    spi_command(sram_mode); // Set mode of operation
    SRAM0_SS = 1; 
    
    // init chip 1
    SRAM1_SS = 0;
    spi_command(WRMR);   // Put into WRITE mode
    spi_command(sram_mode); // Set mode of operation
    SRAM1_SS = 1; 
}

void sram_write(unsigned long sramaddress, unsigned int sramdata) 
{
    // if address is less than the size of the SRAM, use chip 0
    if(sramaddress >= 0 && sramaddress < SRAM_SIZE)
    {
        SRAM0_SS = 0;                     // Enable SRAM
        spi_command(WRITE);      // Send WRITE command, to be followed by 24-bit address
        spi_command(sramaddress >> 16);  // Send first 8 bits of address (16->23)
        spi_command(sramaddress >> 8);   // Send second 8 bits of address (8->15)
        spi_command(sramaddress);        // Send third 8 bits of address (0->7)
        spi_command(sramdata >> 8);      // send upper byte
        spi_command(sramdata);      // send lower byte
        SRAM0_SS = 1;                     // Disable SRAM
    }
    // if address is greater than the size of the SRAM, use chip 1
    else if(sramaddress >= SRAM_SIZE && sramaddress < AVAILABLE_MEMORY)
    {
        sramaddress -= SRAM_SIZE;
        
        SRAM1_SS = 0;                     // Enable SRAM
        spi_command(WRITE);      // Send WRITE command, to be followed by 24-bit address
        spi_command(sramaddress >> 16);  // Send first 8 bits of address (16->23)
        spi_command(sramaddress >> 8);   // Send second 8 bits of address (8->15)
        spi_command(sramaddress);        // Send third 8 bits of address (0->7)
        spi_command(sramdata >> 8);      // send upper byte
        spi_command(sramdata);      // send lower byte
        SRAM1_SS = 1;                     // Disable SRAM
    }
    else
        printf("%lu is an invalid address. Failed to write to SRAM \n", sramaddress);
}

unsigned int sram_read(unsigned long sramaddress) 
{
    unsigned int temp = 0;
    
    // if address is less than the size of the SRAM, use chip 0
    if(sramaddress >= 0 && sramaddress < SRAM_SIZE)
    {
        SRAM0_SS = 0;                           // Enable SRAM
        spi_command(READ);                      // Send WRITE command, to be followed by 24-bit address    
        spi_command(sramaddress >> 16);         // Send first 8 bits of address (16->23)
        spi_command(sramaddress >> 8);          // Send second 8 bits of address (8->15)
        spi_command(sramaddress);               // Send third 8 bits of address (0->7)
        temp = spi_command(255) << 8;           // receive upper byte
        temp |= spi_command(255);               // recieve lower byte and add it to the upper.
        SRAM0_SS = 1;                     // Disable SRAM
    }
    // if address is greater than the size of the SRAM, use chip 1
    else if(sramaddress >= SRAM_SIZE && sramaddress < AVAILABLE_MEMORY)
    {
        sramaddress -= SRAM_SIZE;
        
        SRAM1_SS = 0;                           // Enable SRAM
        spi_command(READ);                      // Send WRITE command, to be followed by 24-bit address    
        spi_command(sramaddress >> 16);         // Send first 8 bits of address (16->23)
        spi_command(sramaddress >> 8);          // Send second 8 bits of address (8->15)
        spi_command(sramaddress);               // Send third 8 bits of address (0->7)
        temp = spi_command(255) << 8;           // receive upper byte
        temp |= spi_command(255);               // recieve lower byte and add it to the upper.
        SRAM1_SS = 1;                     // Disable SRAM
    }
    else
        printf("%lu is an invalid address. Failed to write to SRAM \n", sramaddress);
    
    return temp;                  // Return the received data
}

