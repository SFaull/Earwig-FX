/*
 * File:        effect.c
 * Author:      Sam Faull
 * Description:
 */

#include "config.h"
#include "effect.h"
#include "effects/delay.h"
#include "effects/chorus.h"
#include "effects/distortion.h"
#include "effects/tremolo.h"
#include "effects/bitcrusher.h"
#include "effects/pitchshift.h"
#include <stdbool.h>
#include <stdio.h>

effectInfo_t fx[kEffectCount];
signed int sample;
bool sample_ready;

void effect_init(void)
{       
    effect_set_defaults();
     
    sample = 0;
    sample_ready = false;
    
    config_load();
    
    effect_updateParams();
}

void effect_set_defaults(void)
{
        fx[kDelay].Name = "Delay";
    fx[kDelay].Parameter[0].Name = "Delay Time";
    fx[kDelay].Parameter[0].Unit = "ms";
    fx[kDelay].Parameter[0].Value = 200;
    fx[kDelay].Parameter[0].Min = 100;
    fx[kDelay].Parameter[0].Max = 3000;
    fx[kDelay].Parameter[1].Name = "Feedback";
    fx[kDelay].Parameter[1].Unit = "%";
    fx[kDelay].Parameter[1].Value = 25;
    fx[kDelay].Parameter[1].Min = 1;
    fx[kDelay].Parameter[1].Max = 100;
    fx[kDelay].Enabled = false;
    fx[kDelay].Func = delay; // function pointer to delay function
    

    fx[kDistortion].Name = "Distortion";
    fx[kDistortion].Parameter[0].Name = "Symmetric";
    fx[kDistortion].Parameter[0].Unit = "";
    fx[kDistortion].Parameter[0].Value = 0;
    fx[kDistortion].Parameter[0].Min = 0;
    fx[kDistortion].Parameter[0].Max = 1;
    fx[kDistortion].Parameter[1].Name = "Gain";
    fx[kDistortion].Parameter[1].Unit = "%";
    fx[kDistortion].Parameter[1].Value = 50;
    fx[kDistortion].Parameter[1].Min = 0;
    fx[kDistortion].Parameter[1].Max = 100;
    fx[kDistortion].Enabled = false;
    fx[kDistortion].Func = distortion;
    
    fx[kTremolo].Name = "Tremolo";
    fx[kTremolo].Parameter[0].Name = "Period";
    fx[kTremolo].Parameter[0].Unit = "ms";
    fx[kTremolo].Parameter[0].Value = 200;
    fx[kTremolo].Parameter[0].Min = 100;
    fx[kTremolo].Parameter[0].Max = 2000;
    fx[kTremolo].Enabled = false;
    fx[kTremolo].Func = tremolo;
    
    fx[kChorus].Name = "Chorus";
    fx[kChorus].Parameter[0].Name = "Period";
    fx[kChorus].Parameter[0].Unit = "ms";
    fx[kChorus].Parameter[0].Value = 2000;
    fx[kChorus].Parameter[0].Min = 200;
    fx[kChorus].Parameter[0].Max = 5000;
    fx[kChorus].Enabled = false;
    fx[kChorus].Func = chorus;
    
    fx[kBitcrusher].Name = "Bitcrusher";
    fx[kBitcrusher].Parameter[0].Name = "Bitdepth";
    fx[kBitcrusher].Parameter[0].Unit = "";
    fx[kBitcrusher].Parameter[0].Value = 16;
    fx[kBitcrusher].Parameter[0].Min = 1;
    fx[kBitcrusher].Parameter[0].Max = 16;
    fx[kBitcrusher].Parameter[1].Name = "Fs Divisor";
    fx[kBitcrusher].Parameter[1].Unit = "x";
    fx[kBitcrusher].Parameter[1].Value = 1;
    fx[kBitcrusher].Parameter[1].Min = 1;
    fx[kBitcrusher].Parameter[1].Max = 50;
    fx[kBitcrusher].Enabled = false;
    fx[kBitcrusher].Func = bitcrusher;
    
    fx[kPitchshift].Name = "Pitch Shift";
    fx[kPitchshift].Parameter[0].Name = "Octave";
    fx[kPitchshift].Parameter[0].Unit = "";
    fx[kPitchshift].Parameter[0].Value = 0;
    fx[kPitchshift].Parameter[0].Min = -1;
    fx[kPitchshift].Parameter[0].Max = 1;
    fx[kPitchshift].Parameter[1].Name = "Detune";
    fx[kPitchshift].Parameter[1].Unit = "";
    fx[kPitchshift].Parameter[1].Value = 0;
    fx[kPitchshift].Parameter[1].Min = -1000;
    fx[kPitchshift].Parameter[1].Max = 1000;
    fx[kPitchshift].Enabled = false;
    fx[kPitchshift].Func = pitchshift;
}

void effect_updateParams(void)
{
    // TODO: this function sets to a discrete level - 0-16... could this be more intuitive?
    delay_set_decay((int)(fx[kDelay].Parameter[1].Value * 16 * 0.01));
    delay_set_delay_time(fx[kDelay].Parameter[0].Value);
    distortion_set_percentage(fx[kDistortion].Parameter[1].Value);
    distortion_set_symetric(fx[kDistortion].Parameter[0].Value);
    bitcrusher_setBitdepth(fx[kBitcrusher].Parameter[0].Value);
    bitcrusher_setSampleDivisor(fx[kBitcrusher].Parameter[1].Value);
    chorus_set_period(fx[kChorus].Parameter[0].Value);
    tremolo_set_period(fx[kTremolo].Parameter[0].Value);
    bitcrusher_setOctave(fx[kPitchshift].Parameter[0].Value);
    bitcrusher_setDetune(fx[kPitchshift].Parameter[1].Value);
}

void effect_process(void)
{
    if (sample_ready)
    {
         int i;
         for(i=0; i<kEffectCount; i++)
              if(fx[i].Enabled)
                  sample = fx[i].Func(sample);

         sample_ready = false;
    }
}

int effect_getFxIndexByName(char* str)
{
    int i, j;
    for(i=0; i<kEffectCount; i++)
    {
        char *name = fx[i].Name;
        j = 0;
        bool match = true;
        
        while(str[j] != '\0' && name[j] != '\0') 
        {
            match &= (toupper(name[j]) == str[j]);
            j++;
        }
        
        if (match)
        {
            //printf("FOUND MATCH: %s, %s\n", name, effect);
            return i;
        }
    }
    return -1;  // no match
}

int effect_getParamIndexByName(int effectIndex, char* str)
{
    int i, j;
    for(i=0; i<MAX_PARAMETERS; i++)
    {
        char *name = fx[effectIndex].Parameter[i].Name;
        j = 0;
        bool match = true;
        
        while(str[j] != '\0' && name[j] != '\0') 
        {
            match &= (toupper(name[j]) == str[j]);
            j++;
        }
        
        if (match)
        {
            //printf("FOUND MATCH: %s, %s\n", name, effect);
            return i;
        }
    }
    return -1;  // no match
}

void effect_printEffectList(void)
{
    int i;
    for(i=0; i<kEffectCount; i++)
    {
        if (i <kEffectCount-1)
            printf("%s,", fx[i].Name);
        else 
            printf("%s\n", fx[i].Name);
    }
}