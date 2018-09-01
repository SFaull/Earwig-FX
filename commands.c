#include <xc.h>
#include <stdio.h>
#include "commands.h"
#include "system.h"
#include "parser.h"
#include "version.h"

static void info(void);
static void LED_on(void);
static void LED_off(void);
static void unrecognized(void);

void commands_init(void)
{
    initSerialCommand();
    
    addCommand("*IDN?", info);       
    addCommand("LED:ON", LED_on);       // Turns LED on
    addCommand("LED:OFF", LED_off);       // Turns LED on
    addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
}

static void info(void)
{
    printf("%s, %s\n", MODEL_STRING, FW_VERSION_STR);
}

static void LED_on(void)
{
  printf("LED on\n"); 
  LED = 1;
}

static void LED_off(void)
{
  printf("LED off\n"); 
  LED = 0;
}

static void unrecognized(void)
{
    printf("What?\n"); 
}