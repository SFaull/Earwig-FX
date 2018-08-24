/*
 * File:        selftest.c
 * Author:      Sam Faull
 * Description: Functions for selftest routine
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdbool.h> 
#include "selftest.h"
#include "oled.h"
#include "eeprom.h"
#include "sram.h"

static bool selftest_eeprom();
static bool selftest_sram();
static void selftest_oled();

bool selftest()
{
    bool success = true;
    
    //success = selftest_eeprom();
    //success &= selftest_sram();
    //selftest_oled();
    
    selftest_sram();
    //selftest_eeprom();

    return success;
}

static bool selftest_eeprom()
{
    printf("EEPROM Test \n");
    uint8_t val = 55;
    eeprom_write_byte(15, val);
    printf("EEPROM: Wrote %i \n", val);
    uint8_t response = eeprom_read_byte(15);
    printf("EEPROM: Read %i \n", response);
    if(response == val)
        return true;
    return false;
    
}

static bool selftest_sram()
{
#if 0
    sram_fill(5);
    
    int i;
    for(i=0; i<SRAM_SIZE; i+=2)
    {
        sram_write(i, 5);
        unsigned int val = sram_read(i);
        printf("%i at address %i \n", val, i);
    }
    return false;
#endif
#if 1
    printf("SRAM Test \n");
    uint16_t val = 55;
    sram_write(15, val);
    printf("SRAM: Wrote %i \n", val);
    uint16_t response = sram_read(15);
    printf("SRAM: Read %i \n", response);
    if(response == val)
        return true;
    return false;
#endif
}

static void selftest_oled()
{
    oled_write_string("Starting up...");
    oled_update();
}

