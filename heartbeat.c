#include "heartbeat.h"
#include "timer.h"

#define HEARTBEAT_PERIOD 1000   // every second
#define HEARTBEAT_ONTIME 100    // 1/10 second
#define HEARTBEAT_OFFTIME (HEARTBEAT_PERIOD - HEARTBEAT_ONTIME)    // 9/10 second

static timer_t led_timer;

void heartbeat_init(void)
{
        TRISAbits.TRISA10 = 0;   
        HEARTBEAT_LED = 0;
        timer_start(&led_timer);
}

void heartbeat_process(void)
{
    if(timer_expired(led_timer, HEARTBEAT_OFFTIME) && !HEARTBEAT_LED)
    {
        timer_start(&led_timer);
        HEARTBEAT_LED = 1;  // led on
    }	
    
    if(timer_expired(led_timer, HEARTBEAT_ONTIME) && HEARTBEAT_LED)
    {
        timer_start(&led_timer);
        HEARTBEAT_LED = 0;  // led on
    }	
}