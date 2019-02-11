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
#include "config.h"
#include "watchdog.h"

state_t currentState = kStartup;

static state_t do_Startup(void);
static state_t do_Home(void);
static state_t do_MainMenu(void);
static state_t do_PatchMenu(void);
static state_t do_EffectMenu(void);
static state_t do_SettingsMenu(void);
static state_t do_ParamMenu(void);
static state_t do_ParamEdit(void);

static state_t transition_A(void);
static state_t transition_B(void);
static state_t transition_C(void);
static state_t transition_D(void);
static state_t transition_E(void);
static state_t transition_F(void);
static state_t transition_G(void);
static state_t transition_H(void);
static state_t transition_J(void);
static state_t transition_K(void);
static state_t transition_L(void);
static state_t transition_M(void);


static void init_mainMenu(void);
static void init_patchMenu(void);
static void init_effectMenu(void);
static void refresh_effectMenu(void);
static void init_settingsMenu(void);
static void init_paramMenu();
static void refresh_paramMenu();
static void incValue(void);
static void decValue(void);
static void drawFxChain(void);

static int index;
static effectInfo_t *currentFx;
static int currentParameterValue;

menu_t mainMenu;
menu_t patchMenu;
menu_t effectMenu;
menu_t paramMenu;
menu_t settingsMenu;


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
        case kPatchMenu:
            currentState = do_PatchMenu();
            break;
        case kEffectMenu:
            currentState = do_EffectMenu();
            break;
        case kParamMenu:
            currentState = do_ParamMenu();
            break;
        case kParamEdit:
            currentState = do_ParamEdit();
            break;
        case kSettingsMenu:
            currentState = do_SettingsMenu();
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
    control_t ctrl = navpanel_getControl();
    
    if (ctrl == kOK)
        return transition_B();
    
    if (ctrl == kOKLong)
        return transition_H();
        
    return kHome;
}

static state_t do_MainMenu(void)
{
    int i;
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
            break;
        case kOK:
            i = menu_selectedIndex(&mainMenu);
            if(i==0)
                return transition_J();
            else
                return transition_K();
            break;
        default:
            break;
    }
    
    return kMainMenu;
}


static state_t do_PatchMenu(void)
{
    switch(navpanel_getControl()) 
    {
        case kRotateCW:
            menu_nextPos(&patchMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&patchMenu);
            break;
        case kBack:
            return transition_B();
            break;
        case kOK:
            // TODO 
            break;
        default:
            break;
    }
    
    return kPatchMenu;
}

static state_t do_EffectMenu(void)
{
    switch(navpanel_getControl()) 
    {
        case kRotateCW:
            menu_nextPos(&effectMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&effectMenu);
            break;
        case kBack:
            return transition_B();
        case kOKLong:
            return transition_C();
        case kOK:
            fx[menu_selectedIndex(&effectMenu)].Enabled = (fx[menu_selectedIndex(&effectMenu)].Enabled==0 ?  1 : 0);
            refresh_effectMenu();
            menu_draw(&effectMenu);
            break;
        default:
            break;
    }
    
    return kEffectMenu;
}

static state_t do_SettingsMenu(void)
{
    int i;
    switch(navpanel_getControl()) 
    {
        case kOK:
            i = menu_selectedIndex(&settingsMenu);
            
            if(i == kTestWatchdog)
                watchdog_test();
            
            if(i == kRestoreDefaults)
            {
                effect_set_defaults();
                config_save();
                // TODO: display success messagebox
                return transition_A();
            }
            break;
        case kRotateCW:
            menu_nextPos(&settingsMenu);
            break;
        case kRotateCCW:
            menu_prevPos(&settingsMenu);
            break;
        case kBack:
            return transition_A();
        default:
            break;
    }
    return kSettingsMenu;
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
    effect_saveToConfig();
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
    int i = menu_selectedIndex(&effectMenu);
    currentFx = &fx[i];
    init_paramMenu(currentFx);
    printf("Transition C \n");
    menu_draw(&paramMenu);
    return kParamMenu;
}

static state_t transition_D(void)
{
    printf("Transition D \n");
    menu_draw(&effectMenu);
    return kEffectMenu;
}



static state_t transition_E(void)
{
    index = menu_selectedIndex(&paramMenu);
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
    effect_updateParams();
    return kParamMenu;
}

static state_t transition_G(void)
{
    printf("Transition G \n");
    init_paramMenu();
    menu_draw(&paramMenu);
    return kParamMenu;
}

static state_t transition_H(void)
{
    init_settingsMenu();
    printf("Transition H \n");
    menu_draw(&settingsMenu);
    return kSettingsMenu;
}

static state_t transition_J(void)
{
    printf("Transition J \n");
    init_effectMenu();
    menu_draw(&effectMenu);
    return kEffectMenu;
}

static state_t transition_K(void)
{
    printf("Transition K \n");
    init_patchMenu();
    menu_draw(&patchMenu);
    return kPatchMenu;
}

static state_t transition_L(void)
{
    printf("Transition L \n");
}

static state_t transition_M(void)
{
    printf("Transition M \n");
}

static void init_mainMenu(void)
{
    // setup main menu
    mainMenu.Heading = "Main Menu";
    mainMenu.Item[0] = "Edit Loadout";
    mainMenu.Item[1] = "Save Loadout";
    mainMenu.Item[2] = "Load Patch";

    mainMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    mainMenu.SelectedPosition = 0;  // always select the top item
}


static void init_patchMenu(void)
{
    // setup main menu
    patchMenu.Heading = "Patches";
    patchMenu.Item[0] = "temp...";

    patchMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    patchMenu.SelectedPosition = 0;  // always select the top item
}


static void init_effectMenu(void)
{
    static char temp[kEffectCount][25];
    
    int i;
    // setup main menu
    effectMenu.Heading = "Main Menu";
    for(i=0; i<kEffectCount; i++)
    {
        if (fx[i].Enabled)
            sprintf(temp[i], "[x] %s", fx[i].Name);
        else
            sprintf(temp[i], "[  ] %s", fx[i].Name);
        //paramMenu.Item[i] = malloc(sizeof(char) * 30);
        effectMenu.Item[i] = temp[i];
    }

    effectMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    effectMenu.SelectedPosition = 0;  // always select the top item
}

static void init_settingsMenu(void)
{
    // setup main menu
    settingsMenu.Heading = "Settings";
    settingsMenu.Item[kDeviceInfo] = "Device Info";
    settingsMenu.Item[kTestWatchdog] = "Test Watchdog";
    settingsMenu.Item[kBonusFeatures] = "Bonus Features";
    settingsMenu.Item[kRestoreDefaults] = "Restore Defaults";

    settingsMenu.FirstDisplayedItem = 0;    // First menu item (distortion))
    settingsMenu.SelectedPosition = 0;  // always select the top item
}


static void refresh_effectMenu(void)
{
    static char temp[kEffectCount][25];
    
    int i;
    // setup main menu
    effectMenu.Heading = "Main Menu";
    for(i=0; i<kEffectCount; i++)
    {
        if (fx[i].Enabled)
            sprintf(temp[i], "[x] %s", fx[i].Name);
        else
            sprintf(temp[i], "[  ] %s", fx[i].Name);
        //paramMenu.Item[i] = malloc(sizeof(char) * 30);
        effectMenu.Item[i] = temp[i];
    }
}


static void init_paramMenu()
{
    static char temp[MAX_PARAMETERS][25];   // buffer to hold the string we construct and pass to paramMenu item (if this is not static, it gets wiped and we get nonsense printed to display)
    
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
    static char temp[MAX_PARAMETERS][25];   // buffer to hold the string we construct and pass to paramMenu item (if this is not static, it gets wiped and we get nonsense printed to display)
    
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
    currentParameterValue+=navpanel_getEncoderSteps();
    if (currentParameterValue > currentFx->Parameter[index].Max)
        currentParameterValue = currentFx->Parameter[index].Max;
    refresh_paramMenu();
    menu_draw(&paramMenu);
}

static void decValue(void)
{
    currentParameterValue-=navpanel_getEncoderSteps();
    if(currentParameterValue < currentFx->Parameter[index].Min)
        currentParameterValue = currentFx->Parameter[index].Min;
    refresh_paramMenu();
    menu_draw(&paramMenu);
}

static void drawFxChain(void)
{
    int count = 0;
    char *enabled_fx_names[kEffectCount];
    
    int i;
    for(i=0; i<kEffectCount; i++)
    {
        if(fx[i].Enabled)
        {
            enabled_fx_names[count] = fx[i].Name;
            count++;
        }
    }
    oled_clear();
    //oled_println("Home");
    
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
            //printf("NAME: %s", shorthand_name);
            oled_set_coord(x+(w/2)-(name_width/2), pad+(h/2)-7);
            oled_write_string(shorthand_name);
        }   
    }    
    oled_update();
}