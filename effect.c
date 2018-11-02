/*
 * File:        effect.c
 * Author:      Sam Faull
 * Description:
 */

#include "effect.h"
#include "effects/delay.h"
#include "effects/chorus.h"
#include "effects/distortion.h"
#include "effects/tremolo.h"
#include "effects/bitcrusher.h"
#include <stdbool.h>
#include <stdio.h>

static void setDefaults(void);

effectInfo_t fx[kEffectCount];
signed int sample;
bool sample_ready;

void effect_init(void)
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
    fx[kTremolo].Parameter[0].Name = "Frequency";
    fx[kTremolo].Parameter[0].Unit = "Hz";
    fx[kTremolo].Parameter[0].Value = 4;
    fx[kTremolo].Parameter[0].Min = 1;
    fx[kTremolo].Parameter[0].Max = 20;
    fx[kTremolo].Enabled = false;
    fx[kTremolo].Func = tremolo;
    
    fx[kChorus].Name = "Chorus";
    fx[kChorus].Parameter[0].Name = "Frequency";
    fx[kChorus].Parameter[0].Unit = "Hz";
    fx[kChorus].Parameter[0].Value = 2;
    fx[kChorus].Parameter[0].Min = 1;
    fx[kChorus].Parameter[0].Max = 20;
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
     
    setDefaults();
    effect_updateParams();
}

void setDefaults(void)
{
    tremolo_set_period(50);
    chorus_set_period(50);
    
    sample = 0;
    sample_ready = false;
}

void effect_updateParams()
{
    // TODO: this function sets to a discrete level - 0-16... could this be more intuitive?
    delay_set_decay((int)(fx[kDelay].Parameter[1].Value * 16 * 0.01));
    delay_set_delay_time(fx[kDelay].Parameter[0].Value);
    distortion_set_percentage(fx[kDistortion].Parameter[1].Value);
    distortion_set_symetric(fx[kDistortion].Parameter[0].Value);
    bitcrusher_setBitdepth(fx[kBitcrusher].Parameter[0].Value);
     bitcrusher_setSampleDivisor(fx[kBitcrusher].Parameter[1].Value);
}