/*
 * led_display.h
 *
 *  Created on: Oct 28, 2026
 *      Author: Lenovo
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

void display7SEG(int num);

void setLedBuffer(uint8_t val1, uint8_t val2);

void update7SEG_scan(void);

void clearAllTrafficLeds(void);
void toggleRedLeds(void);
void toggleYellowLeds(void);
void toggleGreenLeds(void);

#endif /* INC_LED_DISPLAY_H_ */