/*
 * File:        effect.c
 * Author:      Sam Faull
 * Description:
 */

#include "effect.h"
#include <stdbool.h>

effect_t fx[2];

void effects_init(void)
{        
    fx[0].Index = 0;
    fx[0].Name = "Delay";
    fx[0].Parameter[0].Name = "Delay Time";
    fx[0].Parameter[0].Unit = "ms";
    fx[0].Parameter[0].Value = 1000;
    fx[0].Parameter[1].Name = "Feedback";
    fx[0].Parameter[1].Unit = "%";
    fx[0].Parameter[1].Value = 25;
    fx[0].Enabled = false;

    fx[1].Index = 1;
    fx[1].Name = "Distortion";
    fx[1].Parameter[0].Name = "Gain";
    fx[1].Parameter[0].Unit = "%";
    fx[1].Parameter[0].Value = 50;
    fx[1].Enabled = false;
}

effect_t effect_get_by_index(int i)
{
    return fx[i];
}