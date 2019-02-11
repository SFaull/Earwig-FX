

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PATCH_H
#define	PATCH_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>

bool patch_exists(int index);
void patch_load(uint16_t address);


#endif	/* PATCH_H */

