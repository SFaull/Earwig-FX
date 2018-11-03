#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "../sram.h"
#include "volume.h"


static int delayVol;
static int delayTime;

// SRAM address pointers
static unsigned long write_address = 0;
static unsigned long read_address = 0;

float delay_get_decay(void) 
{ 
    float percentage = 100-(delayVol*6.25);
    return percentage; 
}

int delay_get_delay_time(void) { return delayTime; }    // returns delaytime in seconds

void delay_set_decay(int decay)
{
    delayVol = decay;
}

// set the delay time in ms
void delay_set_delay_time(int milliseconds)
{
    sram_fill(0); // wipe the RAM contents
    
    unsigned long delaySize = 2*milliseconds/(1000.0/Fs);
    
    // ensure we dont exceed the maximum delay time our memory allows
    if (delaySize > AVAILABLE_MEMORY - 2)
        delaySize = AVAILABLE_MEMORY - 2;
    
    read_address = 0;                          // reset write address
    write_address = delaySize;   // set read according to delay time
    
    // recalculate the delay time now that its actually been implemeted as a number of memory locations
    delayTime = (1000.0/Fs)*((write_address-read_address)/2);
    if(delayTime < 0)
        delayTime *= -1;
    
    printf("Delay: %dms\n", delayTime);
}

signed int delay(signed int delay_in)
{
    signed int delay_out;
    
    //delay_in = delay_in >> 1;   // attenuate sample to avoid clipping
        
    if (read_address >= AVAILABLE_MEMORY)   // Check if max memory location has been reached
        read_address = 0;       // wrap around

    if (write_address >= AVAILABLE_MEMORY)  // Check if max memory location has been reached
        write_address = 0;      // wrap around

    delay_out = sram_read(read_address);   // Read delayed sample from memory
    read_address = read_address + 2;       // increment address pointer
    
    // attenuate delayed sample according to decay factor and add to current sample
    delay_out = volume(delay_out, delayVol) + delay_in;    
    
    sram_write(write_address, delay_out);   // write the new combined sample to memory
    write_address = write_address + 2;      // increment address pointer
    
    return delay_out;
}