#include "chorus.h"
#include "../system.h"
#include <stdbool.h>
#include <math.h> 

#define CHORUS_BUF_SIZE 340

int chorusPeriod = 195; // chorus period used to determine the frequency (default value)
float chorusFreq;

int chorus_get_period(void) { return chorusPeriod; }
float chorus_get_freq(void) { return chorusFreq; }

void chorus_set_period(int period) 
{     
    float timePeriod = period/1000.0;
    float a = timePeriod*Fs;
    int b = 2*CHORUS_BUF_SIZE;
    chorusPeriod = (int)(a/(float)b);

    //printf("Chorus Period:,%d, %lu, %d, %d\n",chorusPeriod);
}

void chorus_set_freq(int freq) 
{ 
    //chorusPeriod = (int)(freq*Fs)/(int)(2*CHORUS_BUF_SIZE);
    float timePeriod = 1/(float)freq;
    float a = timePeriod*Fs;
    int b = 2*CHORUS_BUF_SIZE;
    chorusPeriod = (int)(a/(float)b);

    printf("Chorus Freq: %dHz,%d, %lu, %d, %d\n", freq, Fs, a, b, chorusPeriod);
}

signed int chorus(signed int chorus_in)
{
    static signed int buf[CHORUS_BUF_SIZE];
    static int write;
    static int read;
    static int delay_len;
    static int delay_dir;
    static int LFO = 0;
    
    signed int sample_out;  
    
    //chorus_in = chorus_in >> 1; // halve the sample amplitude to avoid clipping

    read = write - delay_len;   // set read address according to current delay time

    buf[write] = chorus_in;    // store the current sample 
    if (write > CHORUS_BUF_SIZE-1)     // if the final memory location is reached...
        write = 0;          // wrap around
    else
        write++;            // otherwise increment the write pointer

    if (read < 0)   // sometimes the read pointer will be negative, if this is the case
        read = read + CHORUS_BUF_SIZE; // add the buffer length to the read pointer to get the correct memory location

    sample_out = buf[read];    // retrieve the sample and half the amplitude
    read++;                         // increment the read pointer

    if(LFO == chorusPeriod)  // LFO value triggers a change in delay length (currently fixed))
    {
        if (delay_dir == 0) // delay length is increasing
        {
            //rate_led_enabled(false);
            delay_len++;
        }
        else    // delay length is decreasing
        {
            //rate_led_enabled(true);
            delay_len--;
        } 
        LFO = 0;  // reset LFO
    }

    LFO++;  // Increment the LFO counter (this is incremented at the sample rate)

    if (delay_len >= CHORUS_BUF_SIZE-1)    // if the delay length reaches the buffer length
        delay_dir = 1;          // set the delay length to begin decreasing
    if (delay_len <= 1)         // if the delay length reaches a very small value 
        delay_dir = 0;          // set the delay length to begin increasing 

    sample_out = (sample_out>>1) + chorus_in; // add the delayed sample (attenuated) to the current sample

    return sample_out;
}