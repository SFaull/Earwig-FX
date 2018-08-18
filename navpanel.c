/*
 * File:        navpanel.c
 * Author:      Sam Faull
 * Description: Process to capture button pressed and encoder turns
 */

#include "system.h"
#include "navpanel.h"
#include <stdbool.h>

static void initTimer1(void);
static void knobTurned(int L, int R);

static control_t pendingAction;

void navpanel_init(void)
{
    initTimer1();
    pendingAction = kNone;
}

static void initTimer1(void)   // Timer to periodically check encoder pin
{
    T1CONbits.TON = 0;      // Disable Timer
    T1CONbits.TCS = 0;      // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;    // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR1 = 0x00;            // Clear timer register
    PR1 = ENCPRD;           // Load the period value
    IPC0bits.T1IP = 0x01;   // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;      // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Start Timer
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


    if (wait) // wait flag enabled, begin counter (debounce)
    {
        if (cnt >= DEBOUNCE_COUNT)  // 99 counts equates to 0.2 seconds
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


