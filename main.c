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

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    timer_init();
    //uart1_init();
    uart2_init();
    heartbeat_init();
    //wm8510_init(); //initialise WM8510 port       
    //wm8510_config(); //configure WM8510    
    sram_init(seq); //initialise 23LC1024 in sequential mode
    InitI2C1(); //initialise i2c peripheral
    oled_init(); //initialise SSD1306 OLED display
    navpanel_init();
    effect_init();
    
    //selftest();
    
    __delay_ms(1000);    // leave the splashscreen on for a short period
    printf("Ready \n");
    
    commands_init();
    while(1)
    {
        heartbeat_process();
        navpanel_process();
        state_process();
        
        #if 0
        // check for button presses and encoder rotations
        switch(navpanel_getControl())
        {
            case kRotateCW:
                printf("Clockwise \n");
                break;
            case kRotateCCW:
                printf("Counter-Clockwise \n");
                break;
            case kOK:
                printf("OK \n");
                break;
            case kOKLong:
                printf("OK Long \n");
                break;
            case kBack:
                printf("Back \n");
                break;
            case kBackLong:
                printf("Back Long \n");
                break;
            default:
                break;
        }
    #endif
    }
}
