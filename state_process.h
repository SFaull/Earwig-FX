/*
 * File:        state_process.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATE_PROCESS_H
#define	STATE_PROCESS_H

// enumeration for each screen state
typedef enum {
    kStartup = 0,
    kHome = 1,
    kMainMenu = 2,
    kPatchMenu = 3,
    kEffectMenu = 4,
    kParamMenu = 5,
    kParamEdit = 6,
    kSettingsMenu = 7
} state_t;

// enumeration for list of items in settings menu
typedef enum {
    kDeviceInfo = 0,
    kTestWatchdog = 1,
    kBonusFeatures = 2,
    kRestoreDefaults = 3
} setting_t;

void state_process(void);

#endif	/* STATE_PROCESS_H */

