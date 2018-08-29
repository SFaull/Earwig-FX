/*
 * File:        display.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DISPLAY_H
#define	DISPLAY_H

typedef struct
{
    char    *Heading;
    char    *Item[10];
    uint8_t SelectedPosition;
    uint8_t FirstDisplayedItem;
} menu_t;

void display_drawMenu(menu_t *menu);
void display_nextMenuPosition(menu_t *menu);
void display_prevMenuPosition(menu_t *menu);
int display_menuSize(menu_t *menu);

#endif	/* DISPLAY_H */