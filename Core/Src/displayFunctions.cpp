/*
 * displayFunctions.c
 *
 *  Created on: Sep 17, 2019
 *      Author: andre
 */
#include <displayFunctions.hpp>
#include "font.h"

extern SPI_HandleTypeDef hspi1;


void ST7789::printText(const char* text, uint8_t x, uint8_t y, uint16_t color)
{
	uint8_t index = 0;
	while (*(text + index) != '\0') {
		uint8_t character = *(text + index++);
	    for (uint8_t i=0; i<32; i++) {
	        uint8_t byte = font[character][i];
	        for(uint8_t bit=0; bit<8; bit++){
	        	uint8_t pixelOverride = (byte >> bit) & 1;
	        	if(pixelOverride){
	        		drawPixel(bit +((i+1)%2)*8 + ((index-1)*12) + x,y+(i/2), color);

	        	}
	        }
	    }

	}

}

uint16_t ST7789generateColor(uint8_t red, uint8_t green, uint8_t blue){
	return ((blue >> 4) << 10) + ((red >> 3) << 4) + (green >> 4);
}

 void ST7789::writeCommand(uint8_t cmd) {
	//DC low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi1, &cmd, 1, 0);
    //DC high
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    //_HAL_SPI_DISABLE(&hspi1);
}

void ST7789::drawBox(uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint16_t color){
	//Set Address Window to fill up box
	setAddrWindow(xStart, yStart, width, height);
	//Switch LCD to accept image data to ram
	writeCommand(ST77XX_RAMWR);
	//Prepare Color data
	uint8_t tmpColor[2];
	tmpColor[1] = color >> 8;
    tmpColor[0] = color;

    //Use buffer to fill up ram faster
    uint8_t buffer[128];
    for(int i=0; i<128; i++){
    	buffer[i] = tmpColor[i%2];
    }

    //Use DMA to transmit data to LCD
	for(uint16_t i = 0; i < ((width*height)/64)+1; i++){
	  	HAL_SPI_Transmit_DMA(&hspi1, buffer, 128);
	  	while(hspi1.State == HAL_SPI_STATE_BUSY_TX);
	}
}

void ST7789::drawVerticalBar(uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint8_t percentage, uint16_t color, uint8_t update){
	if(!update)
		drawBox(xStart, yStart, width, height, ST77XX_WHITE);
	uint8_t calcHeight = ((height-3)/255.0) * percentage;

	drawBox(xStart+1, yStart+1, width-2, (height-2) - calcHeight, generateColor(10, 10, 10));
	drawBox(xStart+1, yStart + (height-2) - calcHeight, width-2, calcHeight+1, color);
}

void ST7789::drawPixel(uint8_t x, uint8_t y, uint16_t color){
	//Set address window to given coordinates
	setAddrWindow(x, y, 1, 1);
	//Switch LCD to accept image data to ram
	writeCommand(ST77XX_RAMWR);
	//Prepare Color data
	uint8_t tmpColor[2];
	tmpColor[1] = color >> 8;
	tmpColor[0] = color;

	//Transmit single pixel data
	HAL_SPI_Transmit(&hspi1, tmpColor, 2, 0);
}


void ST7789::drawMonochromeBitmap(const uint8_t* bitmap, uint8_t xStart, uint8_t yStart, uint8_t width, uint8_t height, uint16_t color, uint8_t transparent){
	//faster for but destructive method
	if(!transparent){
		setAddrWindow(xStart, yStart, width, height);
		//y value is the same as given by attribute
		for(int y=0; y<height; y++){
			//x value is corrected (8 bit per )
			for(int x=0; x<width; x++){
				//Get current pixel value:
				//byteIndex = y * (bytes per line) + (x / 8)
				//bitIndex  = 8 - (x mod 8)
				uint16_t pixelColor = color * ((bitmap[(y*(width >> 3)) + (x >> 3)] >> (7 - (x%8))) & 1);

				//Prepare Color
				uint8_t tmpColor[2];
				tmpColor[1] = pixelColor >> 8;
				tmpColor[0] = pixelColor;

				//Transmit single pixel data
				HAL_SPI_Transmit(&hspi1,tmpColor, 2, 0);
			}
		}
	}

	//Slower but non destructive (overlay mode)
	else
	{
		for(int y=0; y<height; y++){
			for(int x=0; x<(width/8); x++){
				for(int bit=8; bit>0; bit--){
					uint8_t pixelOverride = ~(bitmap[(y*(width/8)) + (x)] >> (bit-1)) & 1;
					if(pixelOverride){
						drawPixel(xStart+(x*8)+(8-bit), yStart+y, color);
					}

				}
			}
		}
	}

}

static unsigned long x=123456789, y=362436069, z=521288629;
uint16_t ST7789::fastrand() {
	unsigned long t;
	    x ^= x << 16;
	    x ^= x >> 5;
	    x ^= x << 1;

	   t = x;
	   x = y;
	   y = z;
	   z = t ^ x ^ y;
	   return z;
}

void ST7789::SPI_WRITE32(uint32_t data){
	//_HAL_SPI_ENABLE(&hspi1);
	uint8_t tmpData[] = {(uint8_t)(data >> 24), (uint8_t)(data >> 16), (uint8_t)(data >> 8), (uint8_t)(data >> 0)};
	HAL_SPI_Transmit(&hspi1, tmpData, 4, 0);
	//_HAL_SPI_DISABLE(&hspi1);
}

void ST7789::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	uint8_t _xstart = 0;
	uint8_t _ystart = 0;

	x += _xstart;
	y += _ystart;

  uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
  uint32_t ya = ((uint32_t)y << 16) | (y+h-1);

  writeCommand(ST77XX_CASET); // Column addr set

  SPI_WRITE32(xa);

  writeCommand(ST77XX_RASET); // Row addr set

  SPI_WRITE32(ya);

  writeCommand(ST77XX_RAMWR); // write to RAM
}

void ST7789::sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes) {
	//_HAL_SPI_ENABLE(&hspi1);

	//CS high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

	//DC low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &commandByte, 1, 0);

    //DC high
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
      // Send the data bytes
    HAL_SPI_Transmit(&hspi1, dataBytes, numDataBytes, 0);

    //CS high
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    //_HAL_SPI_DISABLE(&hspi1);
}

void ST7789::delay(uint8_t time){
	HAL_Delay(time);
}

void ST7789::displayInit(uint8_t *addr) {

  uint8_t  numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = *(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    cmd = *(addr++);         // Read command
    numArgs  = *(addr++);    // Number of args to follow
    ms       = numArgs & ST_CMD_DELAY;   // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;            // Mask out delay bit
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;
    delay(200);
    if(ms) {
      ms = *(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}
