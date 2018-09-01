/*
 * File:        oled.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OLED_H
#define	OLED_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <string.h> /* memset */
#include <stdbool.h>
#include "system.h"

#define LCDHEIGHT 64
#define LCDWIDTH 128

// Display commands
#define CHARGEPUMP 0x8D
#define COLUMNADDR 0x21
#define COMSCANDEC 0xC8
#define COMSCANINC 0xC0
#define DISPLAYALLON 0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20
#define NORMALDISPLAY 0xA6
#define PAGEADDR 0x22
#define SEGREMAP 0xA0
#define SETCOMPINS 0xDA
#define SETCONTRAST 0x81
#define SETDISPLAYCLOCKDIV 0xD5
#define SETDISPLAYOFFSET 0xD3
#define SETHIGHCOLUMN 0x10
#define SETLOWCOLUMN 0x00
#define SETMULTIPLEX 0xA8
#define SETPRECHARGE 0xD9
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB
#define SWITCHCAPVCC 0x2

#define DISPLAY_BUFFER_SIZE ((LCDHEIGHT*LCDWIDTH)/8)



void oled_i2c_init();
void oled_command(char c);
void oled_data(char c);
void oled_init(void);
void oled_update(void);
void oled_draw_xbm(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const char *xbm);
int oled_get_char_width(char c);
int oled_get_string_width(char *buffer);
void oled_write_char(char c);
void oled_write_string(char *buffer);
void oled_println(char *buffer);
void oled_set_coord(int x, int y);
void oled_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void oled_draw_rect(int16_t x, int16_t y, int16_t width, int16_t height);
void oled_fill_rect(int16_t xMove, int16_t yMove, int16_t width, int16_t height);
void oled_draw_circle(int16_t x0, int16_t y0, int16_t radius);
void oled_fill_circle(int16_t x0, int16_t y0, int16_t radius);
void oled_draw_circle_quads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads);
void oled_draw_horizontal_line(int16_t x, int16_t y, int16_t length);
void oled_draw_vertical_line(int16_t x, int16_t y, int16_t length);
void oled_draw_progress_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);
void oled_set_cursor(int8_t x, int8_t y);
void oled_set_pixel(int16_t x, int16_t y);
void oled_clear(void);
void oled_display_on(bool on);
void oled_invert_display(void);
void oled_normal_display(void);
void oled_set_contrast(char contrast);
void oled_flip_screen_vertically();
void WaitFlag(void);
void WaitForACK(void);
void ResetVariables_I2C(void);


#endif	/* OLED_H */

