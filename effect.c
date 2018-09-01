/*
 * File:        effect.c
 * Author:      Sam Faull
 * Description:
 */

#include "effect.h"
#include <stdbool.h>
#include <stdio.h>

effect_t fx[FX_COUNT];

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
    
    fx[2].Name = "Tremolo";
    fx[2].Parameter[0].Name = "Frequency";
    fx[2].Parameter[0].Unit = "Hz";
    fx[2].Parameter[0].Value = 4;
    fx[2].Parameter[0].Min = 1;
    fx[2].Parameter[0].Max = 20;
    fx[2].Enabled = true;
    
    fx[3].Name = "Chorus";
    fx[3].Parameter[0].Name = "Frequency";
    fx[3].Parameter[0].Unit = "Hz";
    fx[3].Parameter[0].Value = 2;
    fx[3].Parameter[0].Min = 1;
    fx[3].Parameter[0].Max = 20;
    fx[3].Enabled = false;
     
}