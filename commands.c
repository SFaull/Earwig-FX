#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "system.h"
#include "parser.h"
#include "version.h"
#include "navpanel.h"

static void info(void);
static void LED_on(void);
static void LED_off(void);
static void remote_control(void);
static void process_example(void);
static void reset(void);
static void help(void);
static void unrecognized(void);

void commands_init(void)
{
    parser_init();
    
    parser_addCommand("*IDN?", info);      
    parser_addCommand("*RST", reset);   
    parser_addCommand("HELP", help);   
    parser_addCommand("LED:ON", LED_on);       // Turns LED on
    parser_addCommand("LED:OFF", LED_off);       // Turns LED on
    parser_addCommand("CONTROL", remote_control);
    parser_addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
}

static void help(void)
{
    parser_printCommandList();
}

static void info(void)
{
    printf("%s, %s, %s\n", MODEL_STRING, FW_VERSION_STR, BUILD_DATE);
}

static void LED_on(void)
{
  printf("LED on\n"); 
}

static void LED_off(void)
{
  printf("LED off\n"); 
}

static void remote_control(void)
{
  char *arg; 
  arg = parser_next(); 
  if (arg != NULL) 
  {
    if(strcmp("DOWN",arg) == 0)
        navpanel_setControl(kRotateCW);
    else if (strcmp("UP",arg) == 0)
        navpanel_setControl(kRotateCCW);
    else if (strcmp("OK",arg) == 0)
        navpanel_setControl(kOK);
    else if (strcmp("BACK",arg) == 0)
        navpanel_setControl(kBack);
    else if (strcmp("OKHOLD",arg) == 0)
        navpanel_setControl(kOKLong);
    else if (strcmp("BACKHOLD",arg) == 0)
        navpanel_setControl(kBackLong);
    else
        printf("Error: '%s' is not a valid argument", arg);
  } 
  else
      printf("Error: No argument\n");
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

static void reset(void)
{
    asm ("RESET");
}

static void unrecognized(void)
{
    printf("What?\n"); 
}