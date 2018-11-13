#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "system.h"
#include "parser.h"
#include "version.h"
#include "navpanel.h"
#include "effect.h"
#include "eeprom.h"

static void info(void);
static void LED_on(void);
static void LED_off(void);
static void nv_erase(void);
static void remote_control(void);
static void set_effect_parameter(void);
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
    parser_addCommand("NV:ERASE", nv_erase);   
    parser_addCommand("LED:ON", LED_on);       // Turns LED on
    parser_addCommand("LED:OFF", LED_off);       // Turns LED on
    parser_addCommand("CONTROL", remote_control);
    parser_addCommand("SET:EFFECT:PARAM", set_effect_parameter);
    parser_addCommand("GET:SUPPORTEDEFFECTS", effect_printEffectList);
    parser_addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
}

static void help(void)
{
    parser_printCommandList();
}

static void info(void)
{
    printf("%s, %s, %s %s\n", MODEL_STRING, FW_VERSION_STR, BUILD_DATE, BUILD_TIME);
}

static void LED_on(void)
{
  printf("LED on\n"); 
}

static void LED_off(void)
{
  printf("LED off\n"); 
}

static void nv_erase(void)
{
    printf("Erasing NV storage\n"); 
    eeprom_erase();
    printf("Erase complete\n"); 
}

/*
 * Syntax: <effect name> <paramter name> <value>
 */
static void set_effect_parameter(void)
{
    printf("Setting effect param...\n");
    char *effect; 
    char *param;
    char *value;
    int val; 
    int j;

    // store the arguments
    effect = parser_next(); 
    param = parser_next();
    value = parser_next();

    // escape if any arguments not specified
    if (effect == NULL || param == NULL || value == NULL)
    {
        printf("ERROR: Argument syntax\n");
        return;
    }

    // convert the value to an integer
    val = atoi(value);
  
    // look up the effect name
    int fxindex = effect_getFxIndexByName(effect);
    int paramindex = effect_getParamIndexByName(fxindex, param);
    
    if (fxindex == -1 || paramindex == -1)
    {
        printf("ERROR: Argument syntax\n");
        return;
    }
    
    int min, max;
    min = fx[fxindex].Parameter[paramindex].Min;
    max = fx[fxindex].Parameter[paramindex].Max;
    
    if(val >= min && val <= max)
    {
        fx[fxindex].Parameter[paramindex].Value = val;
        effect_updateParams();
        printf("%s %s set to %i", fx[fxindex].Name, fx[fxindex].Parameter[paramindex].Name, fx[fxindex].Parameter[paramindex].Value);
    }
    else
        printf("ERROR: Value out of range\n");
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