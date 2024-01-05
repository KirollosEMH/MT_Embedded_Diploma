/*
 * FanControl.c
 *
 *  Created on: Oct 13, 2023
 *      Author: Kirollos Ehab
 */
#include"lm35_sensor.h"
#include"dcmotor.h"
#include"lcd.h"
#include"adc.h"

int main(void){

	ADC_ConfigType adcConfig;
	adcConfig.ref_volt = INTERNAL_2_56;
	adcConfig.prescaler = FCPU_8;
	ADC_init(&adcConfig);
	LCD_init();
	DcMotor_init();


	LCD_displayString("Fan is ");
	LCD_displayStringRowColumn(1,0,"Temp: ");
	uint8 temp;
	while(1)
	{
		temp = LM35_getTemperature();
		LCD_moveCursor(1,6);

		LCD_intgerToString(temp);
		LCD_displayCharacter(' ');
		LCD_displayString("C ");


		if(temp>=120){
			LCD_displayStringRowColumn(0,7,"ON ");
			DcMotor_Rotate(CW,100);
		}
		else if(temp>=90){
			LCD_displayStringRowColumn(0,7,"ON ");
			DcMotor_Rotate(CW,75);
		}
		else if(temp>=60){
			LCD_displayStringRowColumn(0,7,"ON ");
			DcMotor_Rotate(CW,50);
		}
		else if(temp>=30){
			LCD_displayStringRowColumn(0,7,"ON ");
			DcMotor_Rotate(CW,25);
		}
		else if(temp<30){
			LCD_displayStringRowColumn(0,7,"OFF");
			DcMotor_Rotate(STOP,0);
		}

	}
}
