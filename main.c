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

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    //uart1_init();
    uart2_init();
    LED_init();
    //wm8510_init(); //initialise WM8510 port       
    //wm8510_config(); //configure WM8510    
    sram_init(seq); //initialise 23LC1024 in sequential mode
    InitI2C1(); //initialise i2c peripheral
    oled_init(); //initialise SSD1306 OLED display
    //navpanel_init();
    
    selftest();
    printf("Ready \n");
    
    while(1)
    {
        LED = 1;
        __delay_ms(500);
        LED = 0;
        __delay_ms(500);
        // check for button presses and encoder rotations
		/*
        switch(navpanel_pending_action())
        {
            case kRotateCW:
                break;
            case kRotateCCW:
                break;
            case kOK:
                break;
            case kBack:
                break;
            default:
                break;
        }
		*/
    }
}
