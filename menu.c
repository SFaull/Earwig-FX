/*
 * File:        menu.c
 * Author:      Sam Faull
 * Description: 
 */

#include "oled.h"
#include "menu.h"
#include <stdio.h>
#include "state_process.h"

#define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) ) 
#define MAX_ITEMS 3

void menu_draw(menu_t *menu)
{
    int i;
    const uint8_t temp[3] = {24, 40, 56};
    
    oled_clear();
    oled_println(menu->Heading);
    //oled_draw_rect(0,0,LCDWIDTH-2,(LCDHEIGHT/(MAX_ITEMS+1)-2));
    oled_draw_line(0, (LCDHEIGHT/(MAX_ITEMS+1)-2), LCDWIDTH-3, (LCDHEIGHT/(MAX_ITEMS+1)-2) );
    // for each item in the list, print the content on a new line
    for (i=0; i<MAX_ITEMS; i++)
    {
        oled_write_string("   ");
        oled_println(menu->Item[menu->FirstDisplayedItem + i]);
    }

    // draw the item selction rectangle
    //oled_draw_rect(0,temp[menu->SelectedPosition],LCDWIDTH-7,LCDHEIGHT/(MAX_ITEMS+1)+1);
    oled_draw_circle(3, temp[menu->SelectedPosition], 3);
    /* ---- vertical scroll bar ---- */    
    int itemCount = menu_getCount(menu);
    
    // only draw a scroll bar if number of items in menu exceeds number we can display
    if (itemCount > MAX_ITEMS)
    {
        const uint8_t x = 122;
        const uint8_t y = 14;
        const uint8_t width = 4;
        const uint8_t height = 50;
    
        // do the calculations to get the right sized scroll bar/marker
        int h2 = (height * MAX_ITEMS)/itemCount ;
        int a = itemCount - MAX_ITEMS - 1;
        float ratio = (float)(menu->FirstDisplayedItem)/(float)a;
        int offset = (float)(height-h2)*ratio;
        
        // draw the scrollbar outline
        oled_draw_rect(x, y, width, height);
        // draw the actual bar/marker
        oled_fill_rect(x, y+offset, width, h2);
    }
    
    oled_update();
}

void menu_nextPos(menu_t *menu)
{
    if(menu->SelectedPosition < MAX_ITEMS-1 && menu->SelectedPosition < menu_getCount(menu)-2)
        menu->SelectedPosition++;
    else if ((menu->FirstDisplayedItem + MAX_ITEMS) > ARRAY_SIZE(menu->Item) || strlen(menu->Item[menu->FirstDisplayedItem + MAX_ITEMS]) > 0)
        menu->FirstDisplayedItem++;
    else
        return;
    
    menu_draw(menu);
}

void menu_prevPos(menu_t *menu)
{
    if(menu->SelectedPosition > 0)
        menu->SelectedPosition--;
    else if (menu->FirstDisplayedItem > 0)
        menu->FirstDisplayedItem--;
    else
        return;
    
    menu_draw(menu);
}

int menu_getCount(menu_t *menu)
{
    int i;
    for(i=0; i<ARRAY_SIZE(menu->Item); i++)
    {
        if (menu->Item[i] == NULL)
            break;
    }
    return i+1;
}

int menu_selectedIndex(menu_t *menu)
{
    return (menu->SelectedPosition + menu->FirstDisplayedItem);
}