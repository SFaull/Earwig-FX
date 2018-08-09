/*
 * File:        system.h
 * Author:      Sam Faull
 * Description: System level definitions and declarations
 */

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        7370000L
#define FCY             SYS_FREQ/2

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

