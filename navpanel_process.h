/*
 * File:        navpanel_process.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NAVPANEL_PROCESS_H
#define	NAVPANEL_PROCESS_H

typedef enum {
    kUnknown = 0,
    kHome = 1,
    kMainMenu = 2,
    kParamEdit = 3,
    kChainEdit = 4        
} t_state;


#endif	/* NAVPANEL_PROCESS_H */

