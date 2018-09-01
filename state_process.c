/*
 * File:        state_process.h
 * Author:      Sam Faull
 * Description: 
 */

#include <stdio.h>
#include <string.h>
#include "state_process.h"
#include "navpanel.h"
#include "oled.h"
#include "menu.h"
#include "effect.h"

state_t currentState = kStartup;

static state_t do_Startup(void);
static state_t do_Home(void);
static state_t do_MainMenu(void);
static state_t do_ParamEdit(void);
static state_t do_ChainEdit(void);

static state_t transition_A(void);
static state_t transition_B(void);
static state_t transition_C(void);

static void init_mainMenu(void);
static void init_paramMenu(int index);

menu_t mainMenu;
menu_t paramMenu;


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
    if (navpanel_getControl() == kOK)
        return transition_B();
        
    return kHome;
}

static state_t do_MainMenu(void)
{
    switch(navpanel_getControl()) 
    {
        case kRotateCW:
            menu_nextPos(&mainMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&mainMenu);
            break;
        case kBack:
            return transition_A();
        case kOK:
            return transition_C();
        default:
            break;
    }
    
    return kMainMenu;
}
static state_t do_ParamEdit(void)
{
    switch(navpanel_getControl()) 
    {
        case kRotateCW:
            menu_nextPos(&paramMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&paramMenu);
            break;
        case kBack:
            return transition_B();
        default:
            break;
    }
    return kParamEdit;
}
static state_t do_ChainEdit(void)
{
    return kChainEdit;
}

static state_t transition_A(void)
{
    // TODO: update display with effect chain summary
    printf("Transition A \n");
    oled_clear();
    oled_println("Home");
    oled_update();
    return kHome;
}
static state_t transition_B(void)
{
    // TODO: update display with main menu structure
    init_mainMenu();
    printf("Transition B \n");
    menu_draw(&mainMenu);
    return kMainMenu;
}

static state_t transition_C(void)
{
    int index = mainMenu.SelectedPosition + mainMenu.FirstDisplayedItem;
    init_paramMenu(index);
    printf("Transition C \n");
    menu_draw(&paramMenu);
    return kParamEdit;
}

static void init_mainMenu(void)
{
    int i;
    // setup main menu
    mainMenu.Heading = "Main Menu";
    for(i=0; i<FX_COUNT; i++)
        mainMenu.Item[i] = fx[i].Name;

    mainMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    mainMenu.SelectedPosition = 0;  // always select the top item

}


static void init_paramMenu(int index)
{
    static char temp[MAX_PARAMETERS][30];   // buffer to hold the string we construct and pass to paramMenu item (if this is not static, it gets wiped and we get nonsense printed to display)
    
    // this shows the principal of what the menu should like, but really we want to modify the object
    // therefore we need to retried a reference to it rather than creating a new instance
    
    printf("[%s]\n", fx[index].Name);

    paramMenu.Heading = fx[index].Name;
    
    int i;
    for(i=0; i<MAX_PARAMETERS; i++)  
    {
        if (fx[index].Parameter[i].Name == NULL)
            paramMenu.Item[i] = NULL;
        else
        {
            sprintf(temp[i], "%s: %d%s \0", fx[index].Parameter[i].Name, fx[index].Parameter[i].Value, fx[index].Parameter[i].Unit);
            //paramMenu.Item[i] = malloc(sizeof(char) * 30);
            paramMenu.Item[i] = temp[i];
        }
    }

    paramMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    paramMenu.SelectedPosition = 0;  // always select the top item
}