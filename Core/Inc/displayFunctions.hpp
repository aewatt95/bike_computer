/*
 * displayFunctions.h
 *
 *  Created on: Sep 17, 2019
 *      Author: andre
 */
#include "stm32f1xx_hal.h"

#ifndef DISPLAYFUNCTIONS_H_
#define DISPLAYFUNCTIONS_H_

#define ST_CMD_DELAY      0x80

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_TEOFF      0x34
#define ST77XX_TEON       0x35
#define ST77XX_MADCTL     0x36
#define ST77XX_COLMOD     0x3A

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST7789_CMD_VDVSET 0xc4 // VDV Setting

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD

// Some ready-made 16-bit ('565') color settings:
#define	ST77XX_BLACK      0x0000
#define ST77XX_WHITE      0xFFFF
#define	ST77XX_RED        0xF800
#define	ST77XX_GREEN      0x07E0
#define	ST77XX_BLUE       0x001F
#define ST77XX_CYAN       0x07FF
#define ST77XX_MAGENTA    0xF81F
#define ST77XX_YELLOW     0xFFE0
#define	ST77XX_ORANGE     0xFC00

class ST7789 {
public:
	void sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes);
	void delay(uint8_t time);
	void init();
	void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	void writeCommand(uint8_t cmd);
	void drawBox(uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint16_t color);
	void drawPixel(uint8_t x, uint8_t y, uint16_t color);
	void print(const char* text, uint8_t x, uint8_t y, uint16_t color);
	uint16_t generateColor(uint8_t red, uint8_t green, uint8_t blue);
	void drawMonochromeBitmap(const uint8_t* bitmap, uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint16_t color,  uint8_t transparent);
	void drawVerticalBar(uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint8_t percentage, uint16_t color, uint8_t update);
	void drawHorizontalBar(uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint8_t percentage, uint16_t color, uint8_t update);



private:
	uint8_t generic_st7789[41] =  {        // Init commands for 7789 screens
			11,                              	//  9 commands in list:
			ST77XX_SWRESET,   ST_CMD_DELAY, 	//  1: Software reset, no args, w/delay
			150,                          		//     ~150 ms delay
			ST77XX_SLPOUT ,   ST_CMD_DELAY, 	//  2: Out of sleep mode, no args, w/delay
			10,                          		//      10 ms delay
			ST77XX_COLMOD , 1+ST_CMD_DELAY, 	//  3: Set color mode, 1 arg + delay:
			0x55,                         		//     16-bit color
			10,                           		//     10 ms delay
			ST77XX_MADCTL , 1,              	//  4: Mem access ctrl (directions), 1 arg:
			0x00,                         		//     Row/col addr, bottom-top refresh
			ST77XX_CASET  , 4,              	//  5: Column addr set, 4 args, no delay:
			0x00,
			0,        							//     XSTART = 0
			0,
			240,  								//     XEND = 240
			ST77XX_RASET  , 4,              	//  6: Row addr set, 4 args, no delay:
			0x00,
			0,             						//     YSTART = 0
			320>>8,
			320&0xFF,  							//     YEND = 320
			ST77XX_INVON  ,   ST_CMD_DELAY,  	//  7: hack
			10,
			ST7789_CMD_VDVSET, 1,
			0x20,
			ST77XX_NORON  ,   ST_CMD_DELAY, 	//  8: Normal display on, no args, w/delay
			10,                           		//     10 ms delay
			ST77XX_DISPON ,   ST_CMD_DELAY, 	//  9: Main screen turn on, no args, delay
			10,
			ST77XX_TEON, 0, 0x00};              //    10 ms delay

	void SPI_WRITE32(uint32_t data);
	uint16_t fastrand();

};



#endif /* DISPLAYFUNCTIONS_H_ */
