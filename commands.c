#include <xc.h>
#include <stdio.h>
#include "commands.h"
#include "system.h"
#include "parser.h"

void LED_on(void)
{
  printf("LED on\n"); 
  LED = 1;
}

void LED_off(void)
{
  printf("LED off\n"); 
  LED = 0;
}