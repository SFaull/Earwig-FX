/*
 * File:        effect.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EFFECT_H
#define	EFFECT_H

//#define FX_COUNT 4
#define MAX_PARAMETERS 3

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <stdbool.h>

typedef unsigned int (*Operation)(unsigned int sample);

typedef struct{
    char *Name;
    char *Unit;
    int Value;
    int Min;
    int Max;
} param_t;

typedef struct{
    char *Name;
    param_t Parameter[MAX_PARAMETERS];
    bool Enabled;
    Operation Func; // function pointer
} effectInfo_t;

typedef enum {
    kDistortion = 0,
    kTremolo = 1,
    kDelay = 2,
    kChorus = 3,
    kBitcrusher = 4,
    kPitchshift = 5,
    kEffectCount = 6
} effect_t;

void effect_init(void);
void effect_updateParams(void);
void effect_process(void);
effectInfo_t *effect_get_by_index(int i);
int effect_getFxIndexByName(char* str);
int effect_getParamIndexByName(int effectIndex, char* str);
void effect_printEffectList(void);
void effect_set_defaults(void);
void effect_saveToConfig();
void effect_loadFromConfig(void);
void effect_recording(bool record);


extern effectInfo_t fx[kEffectCount];
extern signed int sample;
extern bool sample_ready;

#endif	/* EFFECT_H */