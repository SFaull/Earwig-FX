#include "tremolo.h"
#include "volume.h"
#include "filter.h"
#include "../system.h"
#include <stdbool.h>

int tremPeriod = 200;   // tremolo period used to determine the frequency (default value)
float tremFreq;

int tremolo_get_period(void) { return tremPeriod; }
float tremolo_get_freq(void) { return tremFreq; }


void tremolo_set_period(int period) 
{ 
    float timePeriod = period/1000.0;
    float a = timePeriod*Fs;
    int b = (2*16)-1;
    tremPeriod = (int)(a/(float)b);
    //printf("Trem samples %d \n", tremPeriod);
}

/*
void tremolo_set_period(int percentage) 
{ 
    
    int maxFreq = 10;
    tremFreq = ((float)percentage * 0.01f * (float)maxFreq);    // received is a percentage so scale accordingly (max 15Hz)
    tremPeriod = (Fs/(tremFreq*31))-1;
    
}
*/
signed int tremolo(signed int trem_in)
{
    static int LFO = 0;
    static int tremVol = 8;      // Volume variable (0: 0% volume, 8: 100% volume)
    static int direction = 0;  
    
    if (LFO == tremPeriod)   // when max LFO is reached (according to trem frequency)
    {
        if (tremVol >= 16)      //Max volume reached
            direction = 0;
        else if (tremVol <= 0)  // min volume reached
            direction = 1;

        if (direction == 1)
        {
            //rate_led_enabled(true);
            tremVol++;
        }
        else
        {
            //rate_led_enabled(false);
            tremVol--;
        }
        LFO = 0;    // reset LFO
    }
    else
        LFO++;
    
    trem_in = volume(trem_in, tremVol); // Attenuate sample according to current volume value
    trem_in = lowpass(trem_in);   // Basic lpf to reduce high freq amplitude transient noise
    
    return trem_in;
}