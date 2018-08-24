/*
 * File:        sram.h
 * Author:      Sam Faull
 * Description: Driver for 23LC1024 SRAM
 */

#include <xc.h>
#include <stdbool.h>
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

void sram_spi_init(void)
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

void sram_fill(unsigned int sramdata) 
{
    unsigned char temp;
    unsigned long a;
    
    SRAM0_SS = 0;                    // Enable SRAM

    SPI2BUF = WRITE;                // Send WRITE command, to be followed by 24-bit address
    while (!SPI2STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI2BUF;                 // Store whatever has been put in the receive buffer

    for (a=0; a<3; a++)             // send 24 bit address in 3 bytes
    {
        SPI2BUF = 0;                    
        while (!SPI2STATbits.SPIRBF);
        temp = SPI2BUF;
    }
    
    for (a=0; a<131071; a++)            // write byte to entire of memory
    {
        SPI2BUF = sramdata;             // Send byte
        while (!SPI2STATbits.SPIRBF);
        temp = SPI2BUF;
    }

    SRAM0_SS = 1;                          // Disable SRAM
}

void sram_init(unsigned char sram_mode)
{
    unsigned char temp;
    
    SRAM0_SS = 0;  // default to SRAM chip #1
    
    SPI2BUF = WRMR;                 // Put into WRITE mode
    while (!SPI2STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI2BUF;                 // Store whatever has been put in the receive buffer

    SPI2BUF = sram_mode;            // Set mode of operation
    while (!SPI2STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI2BUF;                 // Store whatever has been put in the receive buffer

    SRAM0_SS = 1; 
    
    __delay_ms(50);
}

void sram_write(unsigned long sramaddress, unsigned int sramdata) 
{
    unsigned char temp;
    
    SRAM0_SS = 0;                     // Enable SRAM

    SPI2BUF = WRITE;                // Send WRITE command, to be followed by 24-bit address
    while (!SPI2STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI2BUF;                 // Store whatever has been put in the receive buffer

    SPI2BUF = sramaddress >> 16;    // Send first 8 bits of address (16->23)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramaddress >> 8;     // Send second 8 bits of address (8->15)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramaddress;          // Send third 8 bits of address (0->7)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;
    
    SPI2BUF = sramdata >> 8;        // send upper byte
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramdata;             // Send lower byte
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SRAM0_SS = 1;                     // Disable SRAM

}

unsigned int sram_read(unsigned long sramaddress) 
{
    unsigned int temp;

    SRAM0_SS = 0;                     // Enable SRAM

    SPI2BUF = READ;                 // Send WRITE command, to be followed by 24-bit address
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramaddress >> 16;    // Send first 8 bits of address (16->23)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramaddress >> 8;     // Send second 8 bits of address (8->15)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = sramaddress;          // Send third 8 bits of address (0->7)
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF;

    SPI2BUF = 255;
    while (!SPI2STATbits.SPIRBF);
    temp = SPI2BUF << 8;            // receive upper byte

    SPI2BUF = 255;
    while (!SPI2STATbits.SPIRBF);
    temp = temp | SPI2BUF;          // recieve lower byte and add it to the upper.

    SRAM0_SS = 1;                     // Disable SRAM

    return temp;                  // Return the received data
}