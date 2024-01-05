/*
 * Distance.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Dell-G5
 */
#include "ultrasonic.h"
#include "lcd.h"
#include"common_macros.h"
#include<avr/io.h>


int main(void){
	SET_BIT(SREG,7);
	Ultrasonic_init();
	LCD_init();

	uint16 distance;

	LCD_displayStringRowColumn(0,3,"Ultrasonic");
	LCD_displayStringRowColumn(1,1,"Distance: ");
	while(1){
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(1,11);


		LCD_intgerToString(distance);
		LCD_displayString("cm ");



	}
}
