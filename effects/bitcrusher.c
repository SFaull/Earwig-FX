#include "chorus.h"
#include "../system.h"
#include <stdbool.h>
#include <math.h> 

static int bitDepth;
static int sampleDivisor;


unsigned int  bitcrusher(unsigned int sample)
{
    static int counter = 0;
    static signed int prev_sample = 0;
    counter++;
    
    if(counter >= sampleDivisor)
    {
        prev_sample = sample;
        counter = 0;
    }
    
    prev_sample = prev_sample>>(16-bitDepth);
    prev_sample = prev_sample<<(16-bitDepth);
    return prev_sample;
}

void bitcrusher_setBitdepth(int bit_depth)
{
    bitDepth = bit_depth;
}

void bitcrusher_setSampleDivisor(int sample_divisor)
{
    sampleDivisor = sample_divisor;
}