/*
 * File:        main.c
 * Author:      Sam Faull
 * Description: Main c file for Earwig-FX project
 */

#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdbool.h> 

int16_t main(void)
{

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    while(1)
    {

    }
}
