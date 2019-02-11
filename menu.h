/*
 * File:        menu.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MENU_H
#define	MENU_H

typedef struct
{
    char    *Heading;
    char    *Item[100];
    uint8_t SelectedPosition;
    uint8_t FirstDisplayedItem;
} menu_t;

void menu_draw(menu_t *menu);
void menu_nextPos(menu_t *menu);
void menu_prevPos(menu_t *menu);
int menu_getCount(menu_t *menu);
int menu_selectedIndex(menu_t *menu);

#endif	/* MENU_H */