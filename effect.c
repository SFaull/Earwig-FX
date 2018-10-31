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

effect_t fx[FX_COUNT];
signed int sample;
bool sample_ready;

void effect_init(void)
{       
    fx[0].Name = "Delay";
    fx[0].Parameter[0].Name = "Delay Time";
    fx[0].Parameter[0].Unit = "ms";
    fx[0].Parameter[0].Value = 1000;
    fx[0].Parameter[0].Min = 100;
    fx[0].Parameter[0].Max = 3000;
    fx[0].Parameter[1].Name = "Feedback";
    fx[0].Parameter[1].Unit = "%";
    fx[0].Parameter[1].Value = 25;
    fx[0].Parameter[1].Min = 1;
    fx[0].Parameter[1].Max = 100;
    fx[0].Enabled = false;
    fx[0].Func = delay; // function pointer to delay function
    

    fx[1].Name = "Distortion";
    fx[1].Parameter[0].Name = "Symmetric";
    fx[1].Parameter[0].Unit = "";
    fx[1].Parameter[0].Value = 0;
    fx[1].Parameter[0].Min = 0;
    fx[1].Parameter[0].Max = 1;
    fx[1].Parameter[1].Name = "Gain";
    fx[1].Parameter[1].Unit = "%";
    fx[1].Parameter[1].Value = 50;
    fx[1].Parameter[1].Min = 0;
    fx[1].Parameter[1].Max = 100;
    fx[1].Enabled = false;
    fx[1].Func = distortion;
    
    fx[2].Name = "Tremolo";
    fx[2].Parameter[0].Name = "Frequency";
    fx[2].Parameter[0].Unit = "Hz";
    fx[2].Parameter[0].Value = 4;
    fx[2].Parameter[0].Min = 1;
    fx[2].Parameter[0].Max = 20;
    fx[2].Enabled = true;
    fx[2].Func = tremolo;
    
    fx[3].Name = "Chorus";
    fx[3].Parameter[0].Name = "Frequency";
    fx[3].Parameter[0].Unit = "Hz";
    fx[3].Parameter[0].Value = 2;
    fx[3].Parameter[0].Min = 1;
    fx[3].Parameter[0].Max = 20;
    fx[3].Enabled = false;
    fx[3].Func = chorus;
     
    
    setDefaults();
}

void setDefaults(void)
{
    // TODO: All of these functions accept a percentage as the argument - they should be renamed to make this obvious
    distortion_set_percentage(90);
    distortion_set_symetric(true);
    tremolo_set_period(50);
    chorus_set_period(50);
    delay_set_delay_time(5);
    
    // TODO: this function sets to a discrete level - 0-16... could this be more intuitive?
    delay_set_decay(8);
    
    sample = 0;
    sample_ready = false;
}