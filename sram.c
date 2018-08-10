/*
 * File:        sram.h
 * Author:      Sam Faull
 * Description: Driver for 23LC1024 SRAM
 */

#include <xc.h>
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

static unsigned int chip_select; // defaults to SRAM #1

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
    SRAM1_SS = 1;                //disable sram #2
    
    SPI1CON1bits.DISSCK = 0;    //clock enabled (Internal serial clock is enabled)
    SPI1CON1bits.DISSDO = 0;    //SDO enabled (SDOx pin is controlled by the module)
    SPI1CON1bits.MODE16 = 0;    //8 bit mode (set to 1 for 16 bit)
    SPI1CON1bits.CKE = 0;       //data changes on falling edge (active to idle)
    SPI1CON1bits.CKP = 0;       //idle is low
    SPI1CON1bits.PPRE = 3;      //1:1 primary prescaler
    SPI1CON1bits.SPRE = 6;      //2:1 secondary prescaler (20Mhz SPI clock)
    SPI1CON1bits.MSTEN = 1;     //master mode
    SPI1CON1bits.SMP = 0;       //data sampled in middle (Input data is sampled at the middle of data output time)
    SPI1STATbits.SPIEN = 1;     //enable SPI      
}

// call with 0 to select SRAM #1
// call with 1 to select SRAM #2
// any other value will deafult to SRAM #1
void sram_select(unsigned int cs)
{
    if(cs == 1)
        chip_select = SRAM1_SS;
    else
        chip_select = SRAM0_SS;
}

void sram_fill(unsigned int sramdata) 
{
    unsigned char temp;
    unsigned long a;
    
    chip_select = 0;                    // Enable SRAM

    SPI1BUF = WRITE;                // Send WRITE command, to be followed by 24-bit address
    while (!SPI1STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI1BUF;                 // Store whatever has been put in the receive buffer

    for (a=0; a<3; a++)             // send 24 bit address in 3 bytes
    {
        SPI1BUF = 0;                    
        while (!SPI1STATbits.SPIRBF);
        temp = SPI1BUF;
    }
    
    for (a=0; a<131071; a++)            // write byte to entire of memory
    {
        SPI1BUF = sramdata;             // Send byte
        while (!SPI1STATbits.SPIRBF);
        temp = SPI1BUF;
    }

    chip_select = 1;                         // Disable SRAM
}

void sram_init(unsigned char sram_mode)
{
    unsigned char temp;
    
    sram_select(0); // default to SRAM chip #1

    chip_select = 0;                    // Enable SRAM
    SPI1BUF = WRMR;                 // Put into WRITE mode
    while (!SPI1STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI1BUF;                 // Store whatever has been put in the receive buffer

    SPI1BUF = sram_mode;            // Set mode of operation
    while (!SPI1STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI1BUF;                 // Store whatever has been put in the receive buffer

    chip_select = 1;                    // Disable SRAM
    
    sram_fill(0);                   // fill memory with zeros to avoid initial noise
}

void sram_write(unsigned long sramaddress, unsigned int sramdata) 
{
    unsigned char temp;
    
    chip_select = 0;                    // Enable SRAM

    SPI1BUF = WRITE;                // Send WRITE command, to be followed by 24-bit address
    while (!SPI1STATbits.SPIRBF);   // Wait for receive buffer to indicate that its full
    temp = SPI1BUF;                 // Store whatever has been put in the receive buffer

    SPI1BUF = sramaddress >> 16;    // Send first 8 bits of address (16->23)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramaddress >> 8;     // Send second 8 bits of address (8->15)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramaddress;          // Send third 8 bits of address (0->7)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;
    
    SPI1BUF = sramdata >> 8;        // send upper byte
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramdata;             // Send lower byte
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    chip_select = 1;                    // Disable SRAM

}

unsigned int sram_read(unsigned long sramaddress) 
{
    unsigned int temp;

    chip_select = 0;                    // Enable SRAM

    SPI1BUF = READ;                 // Send WRITE command, to be followed by 24-bit address
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramaddress >> 16;    // Send first 8 bits of address (16->23)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramaddress >> 8;     // Send second 8 bits of address (8->15)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = sramaddress;          // Send third 8 bits of address (0->7)
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;

    SPI1BUF = 255;
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF << 8;            // receive upper byte

    SPI1BUF = 255;
    while (!SPI1STATbits.SPIRBF);
    temp = temp | SPI1BUF;          // recieve lower byte and add it to the upper.

    chip_select = 1;                    // Disable SRAM

    return (temp);                  // Return the received data

}