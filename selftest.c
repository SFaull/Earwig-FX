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
    bool success;
    
    success = selftest_eeprom();
    success &= selftest_sram();
    selftest_oled();
    
    return success;
}

static bool selftest_eeprom()
{
    uint8_t val = 55;
    eeprom_write_byte(0x00, val);
    printf("EEPROM: Wrote %i", val);
    uint8_t response = eeprom_read_byte(0x00);
    printf("EEPROM: Read %i", response);
    if(response == val)
        return true;
    return false;
    
}

static bool selftest_sram()
{
    uint16_t val = 55;
    sram_write(0x00, val);
    printf("SRAM: Wrote %i", val);
    uint16_t response = sram_read(0x00);
    printf("SRAM: Read %i", response);
    if(response == val)
        return true;
    return false;
}

static void selftest_oled()
{
    oled_write_string("Starting up...");
    oled_update();
}

