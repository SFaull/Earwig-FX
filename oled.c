/*
 * File:        oled.c
 * Author:      Sam Faull
 * Description: Driver for SSD1306 OLED display
 */

#include <xc.h>
#include "oled.h"
#include <i2c.h>
#include <stdio.h>
#include <string.h> /* memset */
#include <stdbool.h>
#include <stdlib.h>
#include "fonts/Callibri15.h"

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

const uint8_t font[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
	0x00, 0x00, 0x5F, 0x00, 0x00, // !
	0x00, 0x03, 0x00, 0x03, 0x00, // "
	0x14, 0x3E, 0x14, 0x3E, 0x14, // #
	0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
	0x43, 0x33, 0x08, 0x66, 0x61, // %
	0x36, 0x49, 0x55, 0x22, 0x50, // &
	0x00, 0x05, 0x03, 0x00, 0x00, // '
	0x00, 0x1C, 0x22, 0x41, 0x00, // (
	0x00, 0x41, 0x22, 0x1C, 0x00, // )
	0x14, 0x08, 0x3E, 0x08, 0x14, // *
	0x08, 0x08, 0x3E, 0x08, 0x08, // +
	0x00, 0x50, 0x30, 0x00, 0x00, // ,
	0x08, 0x08, 0x08, 0x08, 0x08, // -
	0x00, 0x60, 0x60, 0x00, 0x00, // .
	0x20, 0x10, 0x08, 0x04, 0x02, // /
	0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
	0x00, 0x04, 0x02, 0x7F, 0x00, // 1
	0x42, 0x61, 0x51, 0x49, 0x46, // 2
	0x22, 0x41, 0x49, 0x49, 0x36, // 3
	0x18, 0x14, 0x12, 0x7F, 0x10, // 4
	0x27, 0x45, 0x45, 0x45, 0x39, // 5
	0x3E, 0x49, 0x49, 0x49, 0x32, // 6
	0x01, 0x01, 0x71, 0x09, 0x07, // 7
	0x36, 0x49, 0x49, 0x49, 0x36, // 8
	0x26, 0x49, 0x49, 0x49, 0x3E, // 9
	0x00, 0x36, 0x36, 0x00, 0x00, // :
	0x00, 0x56, 0x36, 0x00, 0x00, // ;
	0x08, 0x14, 0x22, 0x41, 0x00, // <
	0x14, 0x14, 0x14, 0x14, 0x14, // =
	0x00, 0x41, 0x22, 0x14, 0x08, // >
	0x02, 0x01, 0x51, 0x09, 0x06, // ?
	0x3E, 0x41, 0x59, 0x55, 0x5E, // @
	0x7E, 0x09, 0x09, 0x09, 0x7E, // A
	0x7F, 0x49, 0x49, 0x49, 0x36, // B
	0x3E, 0x41, 0x41, 0x41, 0x22, // C
	0x7F, 0x41, 0x41, 0x41, 0x3E, // D
	0x7F, 0x49, 0x49, 0x49, 0x41, // E
	0x7F, 0x09, 0x09, 0x09, 0x01, // F
	0x3E, 0x41, 0x41, 0x49, 0x3A, // G
	0x7F, 0x08, 0x08, 0x08, 0x7F, // H
	0x00, 0x41, 0x7F, 0x41, 0x00, // I
	0x30, 0x40, 0x40, 0x40, 0x3F, // J
	0x7F, 0x08, 0x14, 0x22, 0x41, // K
	0x7F, 0x40, 0x40, 0x40, 0x40, // L
	0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
	0x7F, 0x02, 0x04, 0x08, 0x7F, // N
	0x3E, 0x41, 0x41, 0x41, 0x3E, // O
	0x7F, 0x09, 0x09, 0x09, 0x06, // P
	0x1E, 0x21, 0x21, 0x21, 0x5E, // Q
	0x7F, 0x09, 0x09, 0x09, 0x76, // R
	0x26, 0x49, 0x49, 0x49, 0x32, // S
	0x01, 0x01, 0x7F, 0x01, 0x01, // T
	0x3F, 0x40, 0x40, 0x40, 0x3F, // U
	0x1F, 0x20, 0x40, 0x20, 0x1F, // V
	0x7F, 0x20, 0x10, 0x20, 0x7F, // W
	0x41, 0x22, 0x1C, 0x22, 0x41, // X
	0x07, 0x08, 0x70, 0x08, 0x07, // Y
	0x61, 0x51, 0x49, 0x45, 0x43, // Z
	0x00, 0x7F, 0x41, 0x00, 0x00, // [
	0x02, 0x04, 0x08, 0x10, 0x20, // slash 
	0x00, 0x00, 0x41, 0x7F, 0x00, // ]
	0x04, 0x02, 0x01, 0x02, 0x04, // ^
	0x40, 0x40, 0x40, 0x40, 0x40, // _
	0x00, 0x01, 0x02, 0x04, 0x00, // `
	0x20, 0x54, 0x54, 0x54, 0x78, // a
	0x7F, 0x44, 0x44, 0x44, 0x38, // b
	0x38, 0x44, 0x44, 0x44, 0x44, // c
	0x38, 0x44, 0x44, 0x44, 0x7F, // d
	0x38, 0x54, 0x54, 0x54, 0x18, // e
	0x04, 0x04, 0x7E, 0x05, 0x05, // f
	0x08, 0x54, 0x54, 0x54, 0x3C, // g
	0x7F, 0x08, 0x04, 0x04, 0x78, // h
	0x00, 0x44, 0x7D, 0x40, 0x00, // i
	0x20, 0x40, 0x44, 0x3D, 0x00, // j
	0x7F, 0x10, 0x28, 0x44, 0x00, // k
	0x00, 0x41, 0x7F, 0x40, 0x00, // l
	0x7C, 0x04, 0x78, 0x04, 0x78, // m
	0x7C, 0x08, 0x04, 0x04, 0x78, // n
	0x38, 0x44, 0x44, 0x44, 0x38, // o
	0x7C, 0x14, 0x14, 0x14, 0x08, // p
	0x08, 0x14, 0x14, 0x14, 0x7C, // q
	0x00, 0x7C, 0x08, 0x04, 0x04, // r
	0x48, 0x54, 0x54, 0x54, 0x20, // s
	0x04, 0x04, 0x3F, 0x44, 0x44, // t
	0x3C, 0x40, 0x40, 0x20, 0x7C, // u
	0x1C, 0x20, 0x40, 0x20, 0x1C, // v
	0x3C, 0x40, 0x30, 0x40, 0x3C, // w
	0x44, 0x28, 0x10, 0x28, 0x44, // x
	0x0C, 0x50, 0x50, 0x50, 0x3C, // y
	0x44, 0x64, 0x54, 0x4C, 0x44, // z
	0x00, 0x08, 0x36, 0x41, 0x41, // {
	0x00, 0x00, 0x7F, 0x00, 0x00, // |
	0x41, 0x41, 0x36, 0x08, 0x00, // }
	0x02, 0x01, 0x02, 0x04, 0x02};// ~


int SlaveAddress = 0x3c; 
uint8_t buffer[DISPLAY_BUFFER_SIZE];

void oled_i2c_init()
{
    unsigned int config;
    
     config = (
              I2C1_ON           // I2C Enable bit
            & I2C1_IDLE_CON     // Continue I2C module in Idle mode
            & I2C1_CLK_HLD      // Hold SCL clock low (clock stretch)
            & I2C1_IPMI_DIS     // Intelligent Peripheral Management Interface Enable bit
            & I2C1_7BIT_ADD     // I2CADD is 7-bit address
            & I2C1_SLW_EN       // Enable Slew Rate Control for 400KHz
            & I2C1_SM_DIS       // Disable SMBus input voltage thresholds
            & I2C1_GCALL_DIS    // Disable Interrupt when General call address is received.
            & I2C1_STR_DIS      // SCL Clock Stretch Enable bit (when operating as I2C slave)
            & I2C1_NACK         // Transmit 1 to send NACK as acknowledge
            & I2C1_ACK_DIS      // Acknowledge sequence not in progress
            & I2C1_RCV_DIS      // Receive sequence not in progress
            & I2C1_STOP_DIS     // Stop condition not in progress
            & I2C1_RESTART_DIS  // Repeated Start condition not in progress
            & I2C1_START_DIS    // Start condition not in progress
            );
    OpenI2C1(config, I2C_BRG);          //I2CBRG_VAL = 0x78
    
    IdleI2C1();                 // Wait until I2C Bus is Inactive
}

 void oled_command(char c) {
     
    ResetVariables_I2C();
    StartI2C1();
    WaitFlag();
    
    // I2C command
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    
    MasterWriteI2C1(0x78);
    WaitFlag();
    WaitForACK();
    
    MasterWriteI2C1(control);
    WaitFlag();
    WaitForACK();
    
    MasterWriteI2C1(c);
    WaitFlag();
    WaitForACK();
    
    StopI2C1();	//Send the Stop condition 
    WaitFlag();
    WaitForACK();
    
    IdleI2C1();	//Wait to complete 
}
 void oled_data(char c) {
    ResetVariables_I2C();
    StartI2C1();
    WaitFlag();
    
    uint8_t control = 0x40;   // Co = 0, D/C = 1
    
    MasterWriteI2C1(0x78);
    WaitFlag();
    WaitForACK();
    
    MasterWriteI2C1(control);
    WaitFlag();
    WaitForACK();
    
    MasterWriteI2C1(c);
    WaitFlag();
    WaitForACK();

    
    StopI2C1();	//Send the Stop condition 
    WaitFlag();
    WaitForACK();
    
    IdleI2C1();	//Wait to complete 
}
 
 void oled_init(void) {
  oled_command(DISPLAYOFF);
  oled_command(SETDISPLAYCLOCKDIV);
  oled_command(0xF0); // Increase speed of the display max ~96Hz
  oled_command(SETMULTIPLEX);
  oled_command(0x3F);
  oled_command(SETDISPLAYOFFSET);
  oled_command(0x00);
  oled_command(SETSTARTLINE);
  oled_command(CHARGEPUMP);
  oled_command(0x14);
  oled_command(MEMORYMODE);
  oled_command(0x00);
  oled_command(SEGREMAP);
  oled_command(COMSCANINC);
  oled_command(SETCOMPINS);
  oled_command(0x12);
  oled_command(SETCONTRAST);
  oled_command(0xCF);
  oled_command(SETPRECHARGE);
  oled_command(0xF1);
  oled_command(DISPLAYALLON_RESUME);
  oled_command(NORMALDISPLAY);
  oled_command(0x2e);            // stop scroll
  oled_clear();
  oled_update();    // ensure display has been cleared prior to turning it on
  oled_command(DISPLAYON);
}

void oled_update(void)
{
    uint8_t k;
    uint8_t x;
    uint8_t y;
    uint8_t * p = &buffer[0];
    for (y=0; y<8; y++) 
    {
      oled_command(0xB0+y);
      oled_command(0x02);
      oled_command(0x10);
      for(x=0; x<8; x++) 
      {
        for (k=0; k < 16; k++) 
        {
          oled_data(*p++);
        }
      }
    }
}

void oled_draw_xbm(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const char *xbm) 
{
  int16_t widthInXbm = (width + 7) / 8;
  uint8_t data;
  int16_t y;
  int16_t x;

  for(y = 0; y < height; y++) 
  {
    for(x = 0; x < width; x++ ) 
    {
      if (x & 7) 
      {
        data >>= 1; // Move a bit
      } 
      else 
      {  // Read new data every 8 bit
        data = xbm[(x / 8) + y * widthInXbm];
      }
      // if there is a bit draw it
      if (data & 0x01) 
      {
        oled_set_pixel(xMove + x, yMove + y);
      }
    }
  }
}


int8_t x_cursor = 0;
int8_t y_cursor = 0;


void oled_write_char(char c)
{
  int8_t y;
  int8_t x;
  int8_t data;
  bool thisbit;
  
  int index = c - 32;
  for (x=0; x <5; x++)
  {
      data = font[index*5 + x];
      for(y=0; y<7; y++)
      {
          thisbit = data & (0x00000001 << y);
          if (thisbit)
          {
              oled_set_pixel(x_cursor + x, y_cursor + y);
          }
      }
  }
  
  if (x_cursor < (LCDWIDTH - 5))
  {
    x_cursor += 5;
  }
  else
  {
      x_cursor = 0;
      if (y_cursor < (LCDHEIGHT - 9))
      {
        y_cursor += 9;
      }
      else
      {         
          // TODO find a way to handle the condition where entire screen is full
      }
  }
}

void oled_write_string(char *buffer)
{
    while(*buffer != '\0')    // note: last el of array should be \0
    {      
        oled_write_char(*buffer); // write character
        buffer++;             // go to next character
    }
}

void oled_write_cal_char(char c)
{
  int8_t y;
  int8_t x;
  int8_t data;
  bool thisbit;
  
  int char_index = c - 32;
  int width = Calibri15[Calibri15_FIRST_WIDTH + char_index];
  int index = 0;
  
  int i;
  for (i=0; i<char_index; i++)
  {
      index += (2*Calibri15[i + Calibri15_FIRST_WIDTH]);
  }
  
  for (x=0; x < width; x++)
  {
      data = Calibri15[Calibri15_FIRST_DATA + index + x];
      for(y=0; y<8; y++)
      {
          thisbit = data & (0x00000001 << y);
          if (thisbit)
          {
              oled_set_pixel(x_cursor + x, y_cursor + y);
          }
      }
      
      data = Calibri15[Calibri15_FIRST_DATA + index + x + width];
      for(y=0; y<8; y++)
      {
          thisbit = data & (0x00000001 << y);
          if (thisbit)
          {
              oled_set_pixel(x_cursor + x, y_cursor + y + 7); // TODO why is this 6 and why does it still look kinda crappy?
          }
      }
       
  }
  
  if (x_cursor < (LCDWIDTH - 10))
  {
    x_cursor += (width+1);
  }
  else
  {
      x_cursor = 0;
      if (y_cursor < (LCDHEIGHT - 20))
      {
        y_cursor += 18;
      }
      else
      {         
          // TODO find a way to handle the condition where entire screen is full
      }
  }
}

void oled_write_cal_string(char *buffer)
{
    while(*buffer != '\0')    // note: last el of array should be \0
    {      
        oled_write_cal_char(*buffer); // write character
        buffer++;             // go to next character
    }
}

// Bresenham's algorithm - thx wikipedia and Adafruit_GFX
void oled_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      oled_set_pixel(y0, x0);
    } else {
      oled_set_pixel(x0, y0);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void oled_draw_rect(int16_t x, int16_t y, int16_t width, int16_t height) {
  oled_draw_horizontal_line(x, y, width);
  oled_draw_vertical_line(x, y, height);
  oled_draw_vertical_line(x + width - 1, y, height);
  oled_draw_horizontal_line(x, y + height - 1, width);
}

void oled_fill_rect(int16_t xMove, int16_t yMove, int16_t width, int16_t height) {
  int16_t x;
  for (x = xMove; x < xMove + width; x++) {
    oled_draw_vertical_line(x, yMove, height);
  }
}

void oled_draw_circle(int16_t x0, int16_t y0, int16_t radius) {
  int16_t x = 0, y = radius;
	int16_t dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + 2 * (++x) + 3;
		else
			dp = dp + 2 * (++x) - 2 * (--y) + 5;

		oled_set_pixel(x0 + x, y0 + y);     //For the 8 octants
		oled_set_pixel(x0 - x, y0 + y);
		oled_set_pixel(x0 + x, y0 - y);
		oled_set_pixel(x0 - x, y0 - y);
		oled_set_pixel(x0 + y, y0 + x);
		oled_set_pixel(x0 - y, y0 + x);
		oled_set_pixel(x0 + y, y0 - x);
		oled_set_pixel(x0 - y, y0 - x);

	} while (x < y);

  oled_set_pixel(x0 + radius, y0);
  oled_set_pixel(x0, y0 + radius);
  oled_set_pixel(x0 - radius, y0);
  oled_set_pixel(x0, y0 - radius);
}

void oled_fill_circle(int16_t x0, int16_t y0, int16_t radius) {
  int16_t x = 0, y = radius;
	int16_t dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + 2 * (++x) + 3;
		else
			dp = dp + 2 * (++x) - 2 * (--y) + 5;

    oled_draw_horizontal_line(x0 - x, y0 - y, 2*x);
    oled_draw_horizontal_line(x0 - x, y0 + y, 2*x);
    oled_draw_horizontal_line(x0 - y, y0 - x, 2*y);
    oled_draw_horizontal_line(x0 - y, y0 + x, 2*y);


	} while (x < y);
  oled_draw_horizontal_line(x0 - radius, y0, 2 * radius);

}

void oled_draw_circle_quads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads) {
  int16_t x = 0, y = radius;
  int16_t dp = 1 - radius;
  while (x < y) {
    if (dp < 0)
      dp = dp + 2 * (++x) + 3;
    else
      dp = dp + 2 * (++x) - 2 * (--y) + 5;
    if (quads & 0x1) {
      oled_set_pixel(x0 + x, y0 - y);
      oled_set_pixel(x0 + y, y0 - x);
    }
    if (quads & 0x2) {
      oled_set_pixel(x0 - y, y0 - x);
      oled_set_pixel(x0 - x, y0 - y);
    }
    if (quads & 0x4) {
      oled_set_pixel(x0 - y, y0 + x);
      oled_set_pixel(x0 - x, y0 + y);
    }
    if (quads & 0x8) {
      oled_set_pixel(x0 + x, y0 + y);
      oled_set_pixel(x0 + y, y0 + x);
    }
  }
  if (quads & 0x1 && quads & 0x8) {
    oled_set_pixel(x0 + radius, y0);
  }
  if (quads & 0x4 && quads & 0x8) {
    oled_set_pixel(x0, y0 + radius);
  }
  if (quads & 0x2 && quads & 0x4) {
    oled_set_pixel(x0 - radius, y0);
  }
  if (quads & 0x1 && quads & 0x2) {
    oled_set_pixel(x0, y0 - radius);
  }
}

void oled_draw_horizontal_line(int16_t x, int16_t y, int16_t length) {
  if (y < 0 || y >= LCDHEIGHT) { return; }

  if (x < 0) {
    length += x;
    x = 0;
  }

  if ( (x + length) > LCDWIDTH) {
    length = (LCDWIDTH - x);
  }

  if (length <= 0) { return; }

  uint8_t * bufferPtr = buffer;
  bufferPtr += (y >> 3) * LCDWIDTH;
  bufferPtr += x;

  uint8_t drawBit = 1 << (y & 7);

  while (length--) 
  {
        *bufferPtr++ |= drawBit;
  };
}

void oled_draw_vertical_line(int16_t x, int16_t y, int16_t length) {
  if (x < 0 || x >= LCDWIDTH) return;

  if (y < 0) {
    length += y;
    y = 0;
  }

  if ( (y + length) > LCDHEIGHT) {
    length = (LCDHEIGHT - y);
  }

  if (length <= 0) return;


  uint8_t yOffset = y & 7;
  uint8_t drawBit;
  uint8_t *bufferPtr = buffer;

  bufferPtr += (y >> 3) * LCDWIDTH;
  bufferPtr += x;

  if (yOffset) {
    yOffset = 8 - yOffset;
    drawBit = ~(0xFF >> (yOffset));

    if (length < yOffset) {
      drawBit &= (0xFF >> (yOffset - length));
    }

    *bufferPtr |=  drawBit;

    if (length < yOffset) return;

    length -= yOffset;
    bufferPtr += LCDWIDTH;
  }

  if (length >= 8) {
        drawBit = 0xFF;
        do {
          *bufferPtr = drawBit;
          bufferPtr += LCDWIDTH;
          length -= 8;
        } while (length >= 8);
  }

  if (length > 0) {
    drawBit = (1 << (length & 7)) - 1;
    *bufferPtr |=  drawBit;
  }
}

void oled_draw_progress_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress) {
  uint16_t radius = height / 2;
  uint16_t xRadius = x + radius;
  uint16_t yRadius = y + radius;
  uint16_t doubleRadius = 2 * radius;
  uint16_t innerRadius = radius - 2;

  oled_draw_circle_quads(xRadius, yRadius, radius, 0b00000110);
  oled_draw_horizontal_line(xRadius, y, width - doubleRadius + 1);
  oled_draw_horizontal_line(xRadius, y + height, width - doubleRadius + 1);
  oled_draw_circle_quads(x + width - radius, yRadius, radius, 0b00001001);

  uint16_t maxProgressWidth = (width - doubleRadius - 1) * progress / 100;

  oled_fill_circle(xRadius, yRadius, innerRadius);
  oled_fill_rect(xRadius + 1, y + 2, maxProgressWidth, height - 3);
  oled_fill_circle(xRadius + maxProgressWidth, yRadius, innerRadius);
}

void oled_set_cursor(int8_t x, int8_t y)
{
    x_cursor = x;
    y_cursor = y;
}

void oled_set_pixel(int16_t x, int16_t y) 
{
  if (x >= 0 && x < 128 && y >= 0 && y < 64) 
  {
      buffer[x + (y / 8) * LCDWIDTH] |=  (1 << (y & 7));
  }
}

void oled_clear(void)
{
    memset(buffer, 0, DISPLAY_BUFFER_SIZE);
}

void oled_display_on(bool on) 
{
    if(on)
        oled_command(DISPLAYON);
    else
        oled_command(DISPLAYOFF);
}

void oled_invert_display(void) 
{
  oled_command(INVERTDISPLAY);
}

void oled_normal_display(void) 
{
  oled_command(NORMALDISPLAY);
}

void oled_set_contrast(char contrast) 
{
  oled_command(SETCONTRAST);
  oled_command(contrast);
}

void oled_flip_screen_vertically() 
{
  oled_command(SEGREMAP | 0x01);
  oled_command(COMSCANDEC);           //Rotate screen 180 Deg
}

// I2C functions
void WaitFlag(void)
{
    while(!IFS1bits.MI2C1IF);           // wait for flag to be high
    IFS1bits.MI2C1IF=0;
}
 
void WaitForACK(void)
{
    while(I2C1STATbits.ACKSTAT);        // wait for ack receive from slave
}
 
void ResetVariables_I2C(void)
{
    I2C1CONbits.ACKEN=0;
    I2C1CONbits.PEN=0;
    I2C1CONbits.RCEN=0;
    I2C1CONbits.RSEN=0;
    I2C1CONbits.SEN=0;
}