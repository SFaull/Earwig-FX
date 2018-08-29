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

timer_t led_timer;

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    timer_init();
    //uart1_init();
    uart2_init();
    LED_init();
    //wm8510_init(); //initialise WM8510 port       
    //wm8510_config(); //configure WM8510    
    sram_init(seq); //initialise 23LC1024 in sequential mode
    InitI2C1(); //initialise i2c peripheral
    oled_init(); //initialise SSD1306 OLED display
    navpanel_init();
    effects_init();
    
    //selftest();
    
    __delay_ms(1000);    // leave the splashscreen on for a short period
    printf("Ready \n");
    timer_start(&led_timer);
    while(1)
    {
        navpanel_process();
        state_process();
        
        #if 0
        // check for button presses and encoder rotations
        switch(navpanel_pending_action())
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
        // heartbeat LED
        if(timer_expired(led_timer, 1000))
        {
            timer_start(&led_timer);
            LED = !LED;
        }		
    }
}
