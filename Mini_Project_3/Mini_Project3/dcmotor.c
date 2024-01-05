/*
 * dcmotor.c
 *
 *  Created on: Oct 13, 2023
 *      Author: Kirollos Ehab
 */

#include"dcmotor.h"
#include"pwm.h"



void DcMotor_init(void){
	GPIO_setupPinDirection(DC_INPUT1_PORT_ID,DC_INPUT1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_INPUT2_PORT_ID,DC_INPUT2_PIN_ID,PIN_OUTPUT);

	GPIO_writePin(DC_INPUT1_PORT_ID,DC_INPUT1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DC_INPUT2_PORT_ID,DC_INPUT2_PIN_ID,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	switch (state) {
	case CW:
		GPIO_writePin(DC_INPUT1_PORT_ID,DC_INPUT1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_INPUT2_PORT_ID,DC_INPUT2_PIN_ID,LOGIC_LOW);
		break;
	case ACW:
		GPIO_writePin(DC_INPUT1_PORT_ID,DC_INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_INPUT2_PORT_ID,DC_INPUT2_PIN_ID,LOGIC_HIGH);
		break;
	case STOP:
		GPIO_writePin(DC_INPUT1_PORT_ID,DC_INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_INPUT2_PORT_ID,DC_INPUT2_PIN_ID,LOGIC_LOW);
		break;
	}
	PWM_Timer0_Start(speed);
}
