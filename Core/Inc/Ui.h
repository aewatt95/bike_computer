/*
 * Ui.h
 *
 *  Created on: 09.07.2020
 *      Author: andre
 */

#ifndef SRC_UI_H_
#define SRC_UI_H_




#include <displayFunctions.hpp>




class Ui {
public:
	Ui();
	void init();
	void drawConstElements();
	virtual ~Ui();

private:
ST7789 display;


};


#endif /* SRC_UI_H_ */
