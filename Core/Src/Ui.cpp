/*
 * Ui.cpp
 *
 *  Created on: 09.07.2020
 *      Author: andre
 */

#include "Ui.h"
#include "icons.h"


Ui::Ui() {
	// TODO Auto-generated constructor stub
}

void Ui::init(){
	 display.init();
	 //Clear screen, shadow, main box
	 display.drawBox(0, 0, 240, 240, display.generateColor(0, 0, 0));
	 display.drawBox(40, 60, 180, 140, display.generateColor(20,20,20));
	 display.drawBox(30, 50, 180, 140, display.generateColor(255, 255, 255));
	 display.drawMonochromeBitmap(bike, 80, 70, 80, 64, display.generateColor(230, 230, 230), true);
	 //Loader Bar
	 display.drawBox(30, 150, 180, 40, display.generateColor(50, 80, 50));

	 display.print("Bike Computer", 40, 60, ST77XX_BLACK);
	 display.print("Version 0.1", 45, 85, ST77XX_BLACK);
	 display.print("Andre Eberhard", 32, 110, display.generateColor(10, 10, 10));

	 //Loader
	 for(int i = 0; i<16; i++){
		 display.print(">", 40 + (i*9),160, display.generateColor(0,0,180));
		 display.delay(100);
	 }

	 //Clear Screen
	 display.drawBox(0, 0, 240, 240, ST77XX_BLACK);

}

void Ui::drawConstElements(){
	//Outline
	display.drawBox(0, 0, 240, 20, display.generateColor(20, 20, 20));
	display.drawBox(60, 20, 1, 220, display.generateColor(20, 20, 20));
	display.drawBox(60, 60, 180, 1, display.generateColor(20, 20, 20));

	display.drawBox(60, 85, 180, 1, display.generateColor(20, 20, 20));
	display.drawBox(60, 110, 180, 1, display.generateColor(20, 20, 20));
	display.drawBox(60, 135, 180, 1, display.generateColor(20, 20, 20));
	display.drawBox(60, 160, 180, 1, display.generateColor(20, 20, 20));

	//Bitmaps
	display.drawMonochromeBitmap(bolt, 0, 30, 24, 24, display.generateColor(255, 255, 0), false);
	display.drawMonochromeBitmap(battery, 31, 30, 16, 24, display.generateColor(0, 255, 0), false);
	display.drawMonochromeBitmap(light, 70, 30, 24, 24, display.generateColor(0, 0, 255), false);
	display.drawMonochromeBitmap(usb, 100, 30, 24, 24, display.generateColor(255, 0, 255), false);
	display.drawMonochromeBitmap(breaks, 130, 30, 24, 24, display.generateColor(255, 0, 0), false);

	//Bars
	display.drawVerticalBar(2, 70, 20, 160, 200, display.generateColor(100, 100, 0), false);
	display.drawVerticalBar(30, 70, 20, 160, 179, display.generateColor(0, 100, 0), false);

	display.drawHorizontalBar(70, 170, 160, 20, 127, display.generateColor(100, 100, 0), false);
	display.drawHorizontalBar(70, 210, 160, 20, 100, display.generateColor(0, 100, 0), false);

	display.drawHorizontalBar(172, 32, 50, 20, 200, display.generateColor(20, 20, 20), false);
	//Text
	display.print("Overview", 65, 3, display.generateColor(200, 200, 200));

	//display.print("", 0, 55, ST77XX_WHITE);
	//display.drawPixel(17, 69, ST77XX_WHITE);
	display.print("m/s2", 170, 65, ST77XX_WHITE);
	display.print("km/h", 170, 90, ST77XX_WHITE);
	display.print("km", 170, 115, ST77XX_WHITE);
	display.print("Ws", 170, 140, ST77XX_WHITE);

	display.print("4.2", 70, 65, ST77XX_WHITE);
	display.print("17.2", 70, 90, ST77XX_WHITE);
	display.print("12.65", 70, 115, ST77XX_WHITE);
	display.print("315", 70, 140, ST77XX_WHITE);

/*
	//Dino
	for(int j = 0; j < 30; j++){

		for(int i = 0; i < 20; i++){
			display.drawMonochromeBitmap(dino_bg, 155-i, 46, 24, 8, ST77XX_WHITE, false);
			display.drawBox(140, 30, 25, 25, ST77XX_BLACK);
			display.drawMonochromeBitmap(dino_bg, 205-i, 46, 24, 8, ST77XX_WHITE, false);
			display.drawMonochromeBitmap(dino_bg, 225-i, 46, 24, 8, ST77XX_WHITE, false);
			display.drawBox(220, 30, 25, 25, ST77XX_BLACK);
			display.drawMonochromeBitmap(dino_2, 180, 30, 24, 24, ST77XX_WHITE, false);
			display.delay(100);
		}

	}	//display.drawMonochromeBitmap(dino_bg, 130, 30, 24, 24, ST77XX_WHITE, false);
	//display.drawMonochromeBitmap(dino_bg, 155, 30, 24, 24, ST77XX_WHITE, false);
	//display.drawMonochromeBitmap(dino_bg, 180, 30, 24, 24, ST77XX_WHITE, false);
	//display.drawMonochromeBitmap(dino_1, 130, 30, 24, 24, ST77XX_WHITE, true);

*/
}

Ui::~Ui() {
	// TODO Auto-generated destructor stub
}

