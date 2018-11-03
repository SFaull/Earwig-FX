#include "chorus.h"
#include "../system.h"
#include <stdbool.h>
#include <math.h> 

#define BASE (32767/2)

static long octave_shift = 32767/2;
static long offset_shift = 0;

static void bitcrusher_setShift(int shift);


signed int pitchshift(signed int sample)
{
    #define slength 2048

    long s_shift = octave_shift + offset_shift;
    static int buffer[slength];
    static int writeaddress;
    static int readaddress;
    long sample1;
    long sample2;
    int env;
    static long counter;

    writeaddress = (writeaddress + 1) & (slength - 1);
    buffer[writeaddress] = sample;

    counter = counter + s_shift;
    if (counter > 32767) {
        counter = counter & 32767;
        readaddress = (readaddress + 2) & (slength - 1);
    }

    env = (writeaddress - readaddress) << 5;
    if (env < 0)
        env = ~env;

    sample1 = buffer[readaddress];
    sample2 = buffer[((readaddress + (slength / 2)) & (slength - 1))];
    sample1 = (sample1 * env);
    sample2 = (sample2 * (32767 - env));

    return ((sample1 + sample2) >> 15);
}

static void bitcrusher_setShift(int shift)
{
    
}

void bitcrusher_setOctave(int octave)
{
    if (octave > 0)
        octave_shift = BASE + BASE*octave;
    else if (octave < 0)
        octave_shift = BASE + BASE/(octave*2);
    else
        octave_shift = BASE;
    
    printf("Pitch shift: %l\n", octave_shift+offset_shift);
}

void bitcrusher_setDetune(int detune_shift)
{
    offset_shift = detune_shift;
    printf("Pitch shift: %l\n", octave_shift+offset_shift);
}