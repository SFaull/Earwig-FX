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
static void sram_inc();

bool selftest()
{
    bool success = true;
    
    //success = selftest_eeprom();
    //success &= selftest_sram();
    //selftest_oled();
    
    sram_inc();
    /*
    printf("SRAM 1 \n");
    selftest_sram();
    printf("SRAM 2 \n");
    sram_select(2);
    selftest_sram();
    */
    return success;
}

static bool selftest_eeprom()
{
    uint8_t val = 55;
    eeprom_write_byte(15, val);
    printf("EEPROM: Wrote %i", val);
    uint8_t response = eeprom_read_byte(15);
    printf("EEPROM: Read %i", response);
    if(response == val)
        return true;
    return false;
    
}

static bool selftest_sram()
{
    uint16_t val = 55;
    sram_write(15, val);
    printf("SRAM: Wrote %i \n", val);
    uint16_t response = sram_read(15);
    printf("SRAM: Read %i \n", response);
    if(response == val)
        return true;
    return false;
}

static void sram_inc()
{
    unsigned long i;
    for(i=0; i<20; i+=2)
    {
        sram_write(i, i);
        printf("SRAM: Wrote %lu to address %lu \n", i, i);
        uint16_t response = sram_read(i);
        printf("SRAM: Read %i from address %lu \n", response, i);
    }
}

static void selftest_oled()
{
    oled_write_string("Starting up...");
    oled_update();
}

