/*
 * File:        navpanel.c
 * Author:      Sam Faull
 * Description: Process to capture button pressed and encoder turns
 */

#include "system.h"
#include "navpanel.h"
#include "timer.h"
#include <math.h>
#include <stdbool.h>

static void encoder_process(void);
static void button_process(void);
static void calculate_encoder_speed(void);

static control_t pendingAction;
static timer_t navpanel_update_timer;
static timer_t navpanel_long_press_timer1;
static timer_t navpanel_long_press_timer2;
static timer_t encoder_interval;
static int navpanel_encoder_speed; // steps per second

static int step_LUT[30] = 
{
    1,
    1,
    2,
    3,
    4,
    6,
    8,
    11,
    16,
    23,
    32,
    45,
    64,
    91,
    128,
    181,
    256,
    362,
    512,
    724,
    1024,
    1448,
    2048,
    2896,
    4096,
    5793,
    8192,
    11585,
    16384,
    23170
};

void navpanel_init(void)
{    
    TRISCbits.TRISC0 = 1;      // Rotary encoder pin L
    TRISCbits.TRISC1 = 1;      // Rotary encoder pin R
    TRISCbits.TRISC2 = 1;      // Rotary encoder push button 
    TRISBbits.TRISB5 = 1;      // back button

    pendingAction = kNone;
    
    timer_start(&navpanel_update_timer);
    timer_start(&encoder_interval);
}

int navpanel_getEncoderSteps(void)
{
    return step_LUT[navpanel_encoder_speed-1];
}

control_t navpanel_getControl(void)
{
    control_t action = pendingAction;
    pendingAction = kNone;
    
#if 0
    switch(action)
    {
        case kRotateCW:
            printf("Clockwise \n");
            break;
        case kRotateCCW:
            printf("Counter-Clockwise \n");
            break;
        case kOK:
            printf("OK \n");
            break;
        case kOKLong:
            printf("OK Long \n");
            break;
        case kBack:
            printf("Back \n");
            break;
        case kBackLong:
            printf("Back Long \n");
            break;
        default:
            break;
    }
#endif
    
    return action;
}

void navpanel_setControl(control_t action)
{
    pendingAction = action;
}

void navpanel_process(void)
{    
    if (timer_expired(navpanel_update_timer, 5))  // run this every xms (expected that main loop calls this more regularly))
    {
        timer_start(&navpanel_update_timer);    // restart the timer
        
        encoder_process();
        button_process();
    } 
}

// see https://www.best-microcontroller-projects.com/rotary-encoder.html for rotary encoder algorithm
static void encoder_process(void)
{
    const int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
    static uint8_t prevNextCode = 0;
    static uint16_t store=0;
    
    prevNextCode <<= 2;
    if (ROTARY_L) prevNextCode |= 0x02;
    if (ROTARY_R) prevNextCode |= 0x01;
    prevNextCode &= 0x0f;
     // If valid then store as 16 bit data.
     if  (rot_enc_table[prevNextCode] ) 
     {
        store <<= 4;
        store |= prevNextCode;
        if ((store&0xff)==0x2b) 
        {
            pendingAction = kRotateCW; 
            calculate_encoder_speed();
        }
        if ((store&0xff)==0x17) 
        {
            pendingAction = kRotateCCW; 
            calculate_encoder_speed();
        }
     }
}

static void button_process(void)
{
    static bool button_ok;
    static bool last_button_ok;
    
    static bool button_back;
    static bool last_button_back;
    
    static bool ok_pressed = false;
    static bool back_pressed = false;
    
    // get button states
    last_button_ok = button_ok;     
    button_ok = !(BUTTON_OK);
    last_button_back = button_back;
    button_back = !(BUTTON_BACK);

    // set a flag and start timer on any button press
    if(button_ok && !last_button_ok)
    {
      timer_start(&navpanel_long_press_timer1);
      ok_pressed = true;
    }
    if(button_back && !last_button_back)
    {
      timer_start(&navpanel_long_press_timer2);
      back_pressed = true;
    }

    // check for long presses
    if(button_ok && last_button_ok && ok_pressed && timer_expired(navpanel_long_press_timer1, 500))
    {
      pendingAction = kOKLong;
      ok_pressed = false;
    }
    if(button_back && last_button_back && back_pressed && timer_expired(navpanel_long_press_timer2, 500))
    {
      pendingAction = kBackLong;
      back_pressed = false;
    }

    // check for short releases
    if(!button_ok && last_button_ok && ok_pressed)
    {
        pendingAction = kOK;
        ok_pressed = false;
    }
    if(!button_back && last_button_back && back_pressed)
    {
        pendingAction = kBack;
        back_pressed = false;
    }
}

void calculate_encoder_speed(void)
{
    unsigned long elapsed = timer_elapsed(encoder_interval);
    navpanel_encoder_speed = 1.0/(elapsed/1000.0);
    if (navpanel_encoder_speed <= 0) navpanel_encoder_speed = 1;
    
    timer_start(&encoder_interval);
    
    //printf("Time since last step: %lu \n", elapsed);
    //printf("%d sps\n", navpanel_encoder_speed);
}