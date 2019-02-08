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
#include "oled.h"
#include "i2c.h"
#include "selftest.h"
#include "navpanel.h"
#include "state_process.h"
#include "timer.h"
#include "effect.h"
#include "parser.h"
#include "commands.h"
#include "heartbeat.h"
#include "watchdog.h"
#include "config.h"

#include "effects/chorus.h"

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    timer_init();
    uart1_init();
    uart2_init();
    heartbeat_init();
    wm8510_init(); //initialise WM8510 port       
    wm8510_config(); //configure WM8510    
    sram_init(seq); //initialise 23LC1024 in sequential mode
    InitI2C1(); //initialise i2c peripheral
    navpanel_init();
    commands_init();
    selftest();
    watchdog_init();
    oled_init(); //initialise SSD1306 OLED display
    
    // show a warning if watchdog trip
    if(RCONbits.WDTO)
        oled_wd_splashscreen();
    else
        oled_splashscreen();
    
    config_init();
    effect_init();
    __delay_ms(1000);    // leave the splashscreen on for a short period
    printf("Ready \n");
    

    
    
    while(1)
    {
       watchdog_kick();
       heartbeat_process();
       navpanel_process();
       state_process();
       effect_process();
    }
}