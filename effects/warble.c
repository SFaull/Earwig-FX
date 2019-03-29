#include "warble.h"
#include "../system.h"
#include <stdbool.h>
#include <math.h> 

int warblePeriod;
int sine[WARBLE_BUF_SIZE] = {125,128,130,133,135,138,141,143,
146,148,151,154,156,159,161,164,
166,169,171,173,176,178,181,183,
185,188,190,192,194,196,198,201,
203,205,207,209,211,212,214,216,
218,220,221,223,225,226,228,229,
231,232,233,235,236,237,238,239,
240,241,242,243,244,245,245,246,
247,247,248,248,249,249,249,250,
250,250,250,250,250,250,250,250,
249,249,249,248,248,247,247,246,
245,245,244,243,242,241,240,239,
238,237,236,235,233,232,231,229,
228,226,225,223,221,220,218,216,
214,212,211,209,207,205,203,201,
198,196,194,192,190,188,185,183,
181,178,176,173,171,169,166,164,
161,159,156,154,151,148,146,143,
141,138,135,133,130,128,125,122,
120,117,115,112,109,107,104,102,
99,96,94,91,89,86,84,81,
79,77,74,72,69,67,65,63,
60,58,56,54,52,49,47,45,
43,41,39,38,36,34,32,30,
29,27,25,24,22,21,19,18,
17,15,14,13,12,11,10,9,
8,7,6,5,5,4,3,3,
2,2,1,1,1,0,0,0,
0,0,0,0,0,0,1,1,
1,2,2,3,3,4,5,5,
6,7,8,9,10,11,12,13,
14,15,17,18,19,21,22,24,
25,27,29,30,32,34,36,38,
39,41,43,45,47,49,52,54,
56,58,60,63,65,67,69,72,
74,77,79,81,84,86,89,91,
94,96,99,102,104,107,109,112,
115,117,120,122,125};


void warble_set_period(int period) 
{     
    float timePeriod = period/1000.0;
    float a = timePeriod*Fs;
    int b = 2*WARBLE_BUF_SIZE;
    warblePeriod = (int)(a/(float)b);
    printf("warble period: %d\n", warblePeriod);
}

void warble_set_freq(int freq) 
{ 
    //chorusPeriod = (int)(freq*Fs)/(int)(2*CHORUS_BUF_SIZE);
    float timePeriod = 1/(float)freq;
    float a = timePeriod*Fs;
    int b = 2*WARBLE_BUF_SIZE;
    warblePeriod = (int)(a/(float)b);
    printf("warble period: %d\n", warblePeriod);
}

/* I don't know what an appropriate unit for depth is? perhaps just pick some arbitrary number that sound goof for min/max*/
void warble_set_depth(int percentage) 
{ 
 
}


// The way I understand it is that the LFO controls the speed (warble period) and the amplitude of the sine table is the depth
// BUT depth is also a function of the LFO - ie faster LFO, greater depth

signed int warble(signed int sample_in)
{    
    static signed int buf[WARBLE_BUF_SIZE];
    static int write;
    static int read;
    static int delay_len;
    static int LFO = 0;
    signed int sample_out;  

    read = write - sine[delay_len];   // set read address according to current delay time

    buf[write] = sample_in;    // store the current sample 
    if (write > WARBLE_BUF_SIZE-1)     // if the final memory location is reached...
        write = 0;          // wrap around
    else
        write++;            // otherwise increment the write pointer

    if (read < 0)   // sometimes the read pointer will be negative, if this is the case
        read = read + WARBLE_BUF_SIZE; // add the buffer length to the read pointer to get the correct memory location

    sample_out = buf[read];    // retrieve the sample and half the amplitude
    read++;                         // increment the read pointer

    if(LFO >= warblePeriod)  // LFO value triggers a change in delay length (currently fixed))
    {
        delay_len++;
        LFO = 0;  // reset LFO
    }

    LFO++;  // Increment the LFO counter (this is incremented at the sample rate)

    if (delay_len >= WARBLE_BUF_SIZE-1)    // if the delay length reaches the buffer length
        delay_len = 0;

    //sample_out = (sample_out>>1) + sample_in; // add the delayed sample (attenuated) to the current sample
    
    //printf("dir: %d, lfo: %d, delay: %d\n", delay_dir, LFO, delay_len);

    return sample_out;
}