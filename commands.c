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
#include "config.h"
#include "watchdog.h"
#include "effect.h"

// TODO: rename all to prefix with cmd
static void info(void);
static void LED_on(void);
static void LED_off(void);
static void nv_erase(void);
static void nv_read(void);
static void cmd_nv_write(void);
static void configure_restore_defaults(void);
static void remote_control(void);
static void set_effect_parameter(void);
static void process_example(void);
static void reset(void);
static void help(void);
static void dump(void);
static void cmd_test_watchdog(void);
static void cmd_patch(void);
static void unrecognized(void);

void commands_init(void)
{
    parser_init();
    
    parser_addCommand("*IDN?", info);      
    parser_addCommand("*RST", reset);   
    parser_addCommand("DUMP", dump);   
    parser_addCommand("HELP", help);   
    parser_addCommand("TEST:WD", cmd_test_watchdog); 
    
    parser_addCommand("NV:ERASE", nv_erase);   
    parser_addCommand("NV:READ", nv_read);   
    parser_addCommand("NV:WRITE", cmd_nv_write); 
    
    parser_addCommand("CONF:DEFAULT", configure_restore_defaults); 
    parser_addCommand("CONF:LOAD", config_init); 
    parser_addCommand("CONF:SAVE", effect_saveToConfig); 
    parser_addCommand("CONF:PRINT", config_print); 
    
    parser_addCommand("PATCH", cmd_patch); 
    
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

static void cmd_test_watchdog(void)
{
    watchdog_test();
}

static void cmd_patch(void)
{
    int adress = 40;
    config_get_writable_reference()->Lut.Address[adress] = 0x0300;
    config_save();
}

/**
 * @brief Dump address space
 *
 * Syntax: DUMP [<start> [<qty>]]
 *
 * Where <start> is the start address and <qty> is the number of bytes to dump
 * 
 * Key memory locations:
 * Memory Type      Start Address
 * -------------------------------
 * SFR Space        0x0000
 * X Data RAM       0x0800
 * Y Data RAM       0x2800
 * DMA RAM          0x4000
 * X Data           0x4800  <- address error will occur if you exceed this memory address.
 * Unimplemented    0x8000
 * Progmem          0xAB00
 */
static void dump(void)
{
    char *arg1; 
    char *arg2;
    // store the arguments
    arg1 = parser_next(); 
    arg2 = parser_next();
    

    
	uint8_t * start = (uint8_t *)0x0UL; // start of User program flash memory (Figure 4-1 in datasheet)
	long qty = 128;
	
    // If both arguments were provided
	if(arg1 != NULL)
	{
		long value = strtol(arg1, NULL, 0);
		start = (uint8_t*)value;
	}
		
    // if only 1 arg was provided
	if(arg2 != NULL)
    {
		qty = strtol(arg2, NULL, 0);
    }
	
	char c[17];
	c[0] = 0;
	int j = 0;
    int i;
	for(i = 0 ; i<qty; i++)
	{
		if(i%16 == 0)
		{
			c[j] = 0;
			j = 0;
			if(i)
				printf("  %s\n", c);
			printf(" %08X :", (unsigned int)&start[i]);
		}
		uint8_t d = start[i];
		
		if(isprint ((int)d))
			c[j] = d;
		else
			c[j] = '.';
		j++;	
		printf(" %02X", (unsigned int)d);
	}
	c[j] = 0;
	printf("  %s\n", c);
	return;
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

static void nv_read(void)
{
    char *arg1; 
    char *arg2; 
    arg1 = parser_next(); 
    arg2 = parser_next(); 
    
    if(arg1 == NULL)
	{
        printf("Error: No address specified\n"); 
        return;
    }
    
    int length = 1;
    
    if(arg2 != NULL)
	{
        length = strtol(arg2, NULL, 0);
    }
    
    long value = strtol(arg1, NULL, 0);
    uint16_t address = (uint16_t*)value;
    
    while(length>0)
    {
        
        uint8_t data = eeprom_readByte(address);
        printf("0x%08X: 0x%02X\n", address, (unsigned int)data);
        length--;
        address++;
    }
}

static void cmd_nv_write(void)
{
    char *arg1; 
    char *arg2; 
    arg1 = parser_next(); 
    arg2 = parser_next(); 
    
    if(arg1 == NULL || arg2 == NULL)
	{
        printf("Error\n"); 
        return;
    }
    
    long address = strtol(arg1, NULL, 0);
    long value = strtol(arg2, NULL, 0);
    eeprom_writeByte((unsigned int)address, (unsigned char)value);
}

static void configure_restore_defaults(void)
{
    effect_set_defaults();
    config_applyEffects();
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