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
static state_t do_ParamMenu(void);
static state_t do_ParamEdit(void);

static state_t transition_A(void);
static state_t transition_B(void);
static state_t transition_C(void);
static state_t transition_D(void);
static state_t transition_E(void);
static state_t transition_F(void);
static state_t transition_G(void);

static void init_mainMenu(void);
static void init_paramMenu();
static void refresh_paramMenu();
static void incValue(void);
static void decValue(void);
static void drawFxChain(void);

static int index;
static effect_t *currentFx;
static int currentParameterValue;

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
        case kParamMenu:
            currentState = do_ParamMenu();
            break;
        case kParamEdit:
            currentState = do_ParamEdit();
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
static state_t do_ParamMenu(void)
{
    switch(navpanel_getControl()) 
    {
        case kOK:
            return transition_E();
        case kRotateCW:
            menu_nextPos(&paramMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&paramMenu);
            break;
        case kBack:
            return transition_D();
        default:
            break;
    }
    return kParamMenu;
}

static state_t do_ParamEdit(void)
{
    switch(navpanel_getControl()) 
    {
        case kOK:
            return transition_F();  // save
        case kRotateCW:
            incValue();
            break;
        case kRotateCCW:
            decValue();
            break;
        case kBack:
            return transition_G();  // cancel
        default:
            break;
    }
    return kParamEdit;
}

static state_t transition_A(void)
{
    drawFxChain();
    printf("Transition A \n");
    return kHome;
}
static state_t transition_B(void)
{
    init_mainMenu();
    printf("Transition B \n");
    menu_draw(&mainMenu);
    return kMainMenu;
}

static state_t transition_C(void)
{
    int i = mainMenu.SelectedPosition + mainMenu.FirstDisplayedItem;
    currentFx = &fx[i];
    init_paramMenu(currentFx);
    printf("Transition C \n");
    menu_draw(&paramMenu);
    return kParamMenu;
}

static state_t transition_D(void)
{
    printf("Transition D \n");
    menu_draw(&mainMenu);
    return kMainMenu;
}



static state_t transition_E(void)
{
    index = paramMenu.SelectedPosition + paramMenu.FirstDisplayedItem;
    currentParameterValue = currentFx->Parameter[index].Value;
    printf("Transition E \n");
    // TODO: do something to differentiate edit mode (flash the selection box or something)
    //menu_draw(&paramMenu);
    return kParamEdit;
}

static state_t transition_F(void)
{
    printf("Transition F \n");
    currentFx->Parameter[index].Value = currentParameterValue;
    menu_draw(&paramMenu);
    return kParamMenu;
}

static state_t transition_G(void)
{
    printf("Transition G \n");
    init_paramMenu();  // TODO: fixme
    menu_draw(&paramMenu);
    return kParamMenu;
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


static void init_paramMenu()
{
    static char temp[MAX_PARAMETERS][30];   // buffer to hold the string we construct and pass to paramMenu item (if this is not static, it gets wiped and we get nonsense printed to display)
    
    paramMenu.Heading = currentFx->Name;
    
    int i;
    for(i=0; i<MAX_PARAMETERS; i++)  
    {
        if (currentFx->Parameter[i].Name == NULL)
            paramMenu.Item[i] = NULL;
        else
        {
            sprintf(temp[i], "%s: %d%s \0", currentFx->Parameter[i].Name, currentFx->Parameter[i].Value, currentFx->Parameter[i].Unit);
            //paramMenu.Item[i] = malloc(sizeof(char) * 30);
            paramMenu.Item[i] = temp[i];
        }
    }

    paramMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    paramMenu.SelectedPosition = 0;  // always select the top item
}

static void refresh_paramMenu()
{
    static char temp[MAX_PARAMETERS][30];   // buffer to hold the string we construct and pass to paramMenu item (if this is not static, it gets wiped and we get nonsense printed to display)
    
    paramMenu.Heading = currentFx->Name;
    
    int i;
    for(i=0; i<MAX_PARAMETERS; i++)  
    {
        if (currentFx->Parameter[i].Name == NULL)
            paramMenu.Item[i] = NULL;
        else
        {
            if (i==index)
                sprintf(temp[i], "%s: %d%s \0", currentFx->Parameter[i].Name, currentParameterValue, currentFx->Parameter[i].Unit);
            else
                sprintf(temp[i], "%s: %d%s \0", currentFx->Parameter[i].Name, currentFx->Parameter[i].Value, currentFx->Parameter[i].Unit);
            //paramMenu.Item[i] = malloc(sizeof(char) * 30);
            paramMenu.Item[i] = temp[i];
        }
    }
}

static void incValue(void)
{
    if (currentParameterValue < currentFx->Parameter[index].Max)
    {
        currentParameterValue++;
        refresh_paramMenu();
        menu_draw(&paramMenu);
    }
}

static void decValue(void)
{
    if (currentParameterValue > currentFx->Parameter[index].Min)
    {
        currentParameterValue--;
        refresh_paramMenu();
        menu_draw(&paramMenu);
    }
}

static void drawFxChain(void)
{
    int count = 0;
    char *enabled_fx_names[FX_COUNT];
    
    int i;
    for(i=0; i<FX_COUNT; i++)
    {
        if(fx[i].Enabled)
        {
            enabled_fx_names[count] = fx[i].Name;
            count++;
        }
    }
    oled_clear();
    oled_println("Home");
    
    if (count == 0)
        oled_println("No Effects...");
    else
    {
        for(i=0; i<count; i++)
        {
            int pad = 20;
            int x = (LCDWIDTH/count)*i;
            int y = pad;
            int w = (LCDWIDTH-pad)/count;
            int h = LCDHEIGHT-(2*pad);
            oled_draw_rect(x,y,w,h);

            char shorthand_name[5];     // Destination string
            strncpy(shorthand_name, enabled_fx_names[i], 4);
            shorthand_name[4] = 0; // null terminate destination
            int name_width = oled_get_string_width(shorthand_name);
            printf("NAME: %s", shorthand_name);
            oled_set_coord(x+(w/2)-(name_width/2), pad+(h/2)-7);
            oled_write_string(shorthand_name);
        }   
    }    
    oled_update();
}