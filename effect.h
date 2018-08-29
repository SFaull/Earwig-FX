/*
 * File:        effect.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EFFECT_H
#define	EFFECT_H

#define MAX_PARAMETERS 3

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdbool.h>

typedef struct{
    char *Name;
    char *Unit;
    int Value;
} param_t;

typedef struct{
    uint8_t Index;
    char *Name;
    param_t Parameter[MAX_PARAMETERS];
    bool Enabled;
} effect_t;

void effects_init(void);
effect_t effect_get_by_index(int i);

#endif	/* EFFECT_H */