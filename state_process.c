/*
 * File:        state_process.h
 * Author:      Sam Faull
 * Description: 
 */

#include <stdio.h>
#include "state_process.h"
#include "navpanel.h"
#include "oled.h"

state_t currentState = kStartup;

static state_t do_Startup(void);
static state_t do_Home(void);
static state_t do_MainMenu(void);
static state_t do_ParamEdit(void);
static state_t do_ChainEdit(void);

static state_t transition_A(void);
static state_t transition_B(void);

uint8_t position;

void state_process(void)
{
    switch(currentState)
    {
        case kStartup:
            currentState = do_Startup();
            break;
        case kHome:
            currentState = do_Home();
            break;
        case kMainMenu:
            currentState = do_MainMenu();
            break;
        case kParamEdit:
            currentState = do_ParamEdit();
            break;
        case kChainEdit:
            currentState = do_ChainEdit();
            break;
        default:
            currentState = kHome;
            break;
    }
}

static state_t do_Startup(void)
{
    return transition_A();
}
static state_t do_Home(void)
{
    if (navpanel_pending_action() == kOK)
        return transition_B();
        
    return kHome;
}

static state_t do_MainMenu(void)
{
    switch(navpanel_pending_action()) 
    {
        case kRotateCW:
            nextPosition();
            drawMenu();
            break;
        case kRotateCCW:
            prevPosition();
            drawMenu();
            break;
        case kBack:
            return transition_A();
    }
    
    return kMainMenu;
}
static state_t do_ParamEdit(void)
{
    return kParamEdit;
}
static state_t do_ChainEdit(void)
{
    return kChainEdit;
}

static state_t transition_A(void)
{
    // TODO: update display with effect chain summary
    printf("Transition to Home Screen \n");
    oled_clear();
    oled_println("Home");
    oled_update();
    return kHome;
}
static state_t transition_B(void)
{
    // TODO: update display with main menu structure
    printf("Transition to Menu Screen \n");
    position = 0;
    drawMenu();
    return kMainMenu;
}

void drawMenu()
{
    const uint8_t temp[3] = {15, 31, 47};
    oled_clear();
    oled_println("Menu Menu");
    oled_println("Item 1");
    oled_println("Item 2");
    oled_println("Item 3");
    oled_draw_rect(0,temp[position],126,16);
    oled_update();
}

void nextPosition()
{
    if(position < 2)
        position++;
}

void prevPosition()
{
    if(position > 0)
        position--;
}