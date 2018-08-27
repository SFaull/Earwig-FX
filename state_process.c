/*
 * File:        state_process.h
 * Author:      Sam Faull
 * Description: 
 */

#include <stdio.h>
#include "state_process.h"
#include "navpanel.h"
#include "oled.h"
#include "display.h"

state_t currentState = kStartup;

static state_t do_Startup(void);
static state_t do_Home(void);
static state_t do_MainMenu(void);
static state_t do_ParamEdit(void);
static state_t do_ChainEdit(void);

static state_t transition_A(void);
static state_t transition_B(void);

static void init_mainMenu(void);

menu_t mainMenu;

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
            display_nextMenuPosition(&mainMenu);
            break;
        case kRotateCCW:
            display_prevMenuPosition(&mainMenu);
            break;
        case kBack:
            return transition_A();
        default:
            break;
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
    init_mainMenu();
    printf("Transition to Menu Screen \n");
    display_drawMenu(&mainMenu);
    return kMainMenu;
}


static void init_mainMenu(void)
{
    // setup main menu
    mainMenu.Heading = "Main Menu";
    mainMenu.Item[0] = "Distortion";
    mainMenu.Item[1] = "Delay";
    mainMenu.Item[2] = "Tremolo";
    mainMenu.Item[3] = "Chorus";
    mainMenu.Item[4] = "Phaser";
    mainMenu.Item[5] = "Fuzz";
    mainMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    mainMenu.SelectedPosition = 0;  // always select the top item

}