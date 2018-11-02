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
    fx[kDelay].Parameter[0].Value = 1000;
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
     
    setDefaults();
    effect_updateParams();
}

void setDefaults(void)
{
    // TODO: All of these functions accept a percentage as the argument - they should be renamed to make this obvious
    distortion_set_percentage(90);
    distortion_set_symetric(true);
    tremolo_set_period(50);
    chorus_set_period(50);
    delay_set_delay_time(400);
    
    // TODO: this function sets to a discrete level - 0-16... could this be more intuitive?
    delay_set_decay(4);
    
    sample = 0;
    sample_ready = false;
}

void effect_updateParams()
{
    // TODO: this function sets to a discrete level - 0-16... could this be more intuitive?
    delay_set_decay((int)(fx[kDelay].Parameter[1].Value * 16 * 0.01));
    delay_set_delay_time(fx[kDelay].Parameter[0].Value);
    //distortion_set_percentage(fx[kDistortion].Parameter[1].Value);
    //distortion_set_symetric(fx[kDistortion].Parameter[0].Value);
    
    //tremolo_set_period(50);
    //chorus_set_period(50);
    
    
}