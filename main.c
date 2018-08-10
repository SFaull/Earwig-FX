/*
 * File:        main.c
 * Author:      Sam Faull
 * Description: Main c file for Earwig-FX project
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdbool.h> 
#include "system.h"
#include "uart.h"
#include "wm8510.h"
#include "sram.h"

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    uart1_init();
    uart2_init();
    wm8510_init(); //initialise WM8510 port       
    wm8510_config(); //configure WM8510    
    sram_spi_init(); // initialise spi peripheral
    sram_init(seq); //initialise 23LC1024 in sequential mode
    oled_i2c_init(); //initialise i2c peripheral
    oled_init(); //initialise SSD1306 OLED display
    
    while(1)
    {

    }
}
