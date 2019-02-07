/*
 * File:        config.h
 * Author:      Sam Faull
 * Description: 
 */

#include <stdbool.h>

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIG_H
#define	CONFIG_H

bool config_load(void);
void config_save(void);

#endif	/* CONFIG_H */