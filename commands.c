#include <xc.h>
#include <stdio.h>
#include "commands.h"
#include "system.h"
#include "parser.h"
#include "version.h"

static void info(void);
static void LED_on(void);
static void LED_off(void);
static void process_example(void)  ;
static void unrecognized(void);

void commands_init(void)
{
    parser_init();
    
    parser_addCommand("*IDN?", info);       
    parser_addCommand("LED:ON", LED_on);       // Turns LED on
    parser_addCommand("LED:OFF", LED_off);       // Turns LED on
    parser_addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
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

static void process_example(void)    
{
  char *arg; 

  arg = parser_next(); 
  if (arg != NULL) 
  {
    printf("Arguments received: %s", arg);
  } 
}

static void unrecognized(void)
{
    printf("What?\n"); 
}