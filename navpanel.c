/*
 * File:        navpanel.c
 * Author:      Sam Faull
 * Description: Process to capture button pressed and encoder turns
 */

#include "system.h"
#include "navpanel.h"
#include "timer.h"
#include <stdbool.h>

static void knobTurned(int L, int R);

static control_t pendingAction;

timer_t navpanel_update_timer;

void navpanel_init(void)
{    
    TRISCbits.TRISC0 = 1;      // Rotary encoder pin L
    TRISCbits.TRISC1 = 1;      // Rotary encoder pin R
    TRISCbits.TRISC2 = 1;      // Rotary encoder push button 
    TRISBbits.TRISB5 = 1;      // back button

    pendingAction = kNone;
    
    timer_start(&navpanel_update_timer);
}

control_t navpanel_pending_action(void)
{
    control_t action = pendingAction;
    pendingAction = kNone;
    return action;
}

void navpanel_process(void)
{
    static bool wait;        // flag used to debounce
    static int cnt;         // counter used to debounce
    
    static bool enc;         // !encoder pin value
    static bool last_enc;    // previous !encoder pin value
    
    static bool button_ok;
    static bool last_button_ok;
    
    static bool button_back;
    static bool last_button_back;
    
    if (timer_expired(navpanel_update_timer, 5))  // run this every 20ms (expected that main loop calls this more regularly))
    {
        timer_start(&navpanel_update_timer);    // restart the timer
        if (wait) // wait flag enabled, begin counter (debounce)
        {
            if (cnt >= DEBOUNCE_COUNT)  
            {
                cnt = 0;    // reset counter
                wait = false;   // reset wait flag
            }
            else
                cnt++;      // increment counter
        }
        else    // otherwise, check the encoder pin
        {
            // get encoder state
            last_enc = enc;
            enc = !(ROTARY_L); 

            // get button states
            last_button_ok = button_ok;     
            button_ok = !(BUTTON_OK);
            last_button_back = button_back;
            button_back = !(BUTTON_BACK);

            // look for state transitions
            if(!enc && last_enc) // If a change has occured
            {
              knobTurned(ROTARY_L, ROTARY_R);   // send values to knobturned function
              wait = true; // set wait flag so no turns are registered for 0.2 seconds (debounce)
            }

            if(!button_ok && last_button_ok) // If a change has occured
            {
              pendingAction = kOK;
              wait = true; // set wait flag so no turns are registered for 0.2 seconds (debounce)
            }

            if(!button_back && last_button_back) // If a change has occured
            {
              pendingAction = kBack;
              wait = true; // set wait flag so no turns are registered for 0.2 seconds (debounce)
            }
        } 
    }



}

static void knobTurned(int L, int R)
{    
    static int state = 0;           // will store two bits for pins A & B on the encoder which we will get from the pins above
    static int bump[] = {0,0,1,-1};
    
    state = 0;          // reset each time
    state = state + L;  // add the state of Pin L
    state <<= 1;        // shift the bit over one spot
    state = state + R;  // add the state of Pin R
 
     // posible states:
     //00 & 01 - something is wrong, do nothing
     //10 - knob was turned forwards
     //11 - knob was turned backwards
     
     switch(bump[state])
     {
         case 1:    pendingAction = kRotateCW;  break;
         case -1:   pendingAction = kRotateCCW; break;
         default:   break;
     }
}


