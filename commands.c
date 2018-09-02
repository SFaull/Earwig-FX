#include <xc.h>
#include <stdio.h>
#include "commands.h"
#include "system.h"
#include "parser.h"
#include "version.h"
#include "navpanel.h"

static void info(void);
static void LED_on(void);
static void LED_off(void);
static void remote_control(void)  ;
static void process_example(void)  ;
static void unrecognized(void);

void commands_init(void)
{
    parser_init();
    
    parser_addCommand("*IDN?", info);       
    parser_addCommand("LED:ON", LED_on);       // Turns LED on
    parser_addCommand("LED:OFF", LED_off);       // Turns LED on
    parser_addCommand("CONTROL", remote_control);
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

static void remote_control(void)
{
  char *arg; 
  arg = parser_next(); 
  if (arg != NULL) 
  {
    if(strcmp("UP",arg) != 0)
        navpanel_setControl(kRotateCW);
    else if (strcmp("DOWN",arg) != 0)
        navpanel_setControl(kRotateCCW);
    else if (strcmp("OK",arg) != 0)
        navpanel_setControl(kOK);
    else if (strcmp("BACK") != 0)
        navpanel_setControl(kBack);
    else if (strcmp("OKHOLD",arg) != 0)
        navpanel_setControl(kOKLong);
    else if (strcmp("BACKHOLD",arg) != 0)
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

static void unrecognized(void)
{
    printf("What?\n"); 
}