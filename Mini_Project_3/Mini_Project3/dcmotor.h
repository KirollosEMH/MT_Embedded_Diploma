/*
 * dcmotor.h
 *
 *  Created on: Oct 13, 2023
 *      Author: Kirollos Ehab
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include"std_types.h"
#include"gpio.h"

#define DC_INPUT1_PORT_ID	PORTB_ID
#define DC_INPUT2_PORT_ID	PORTB_ID

#define DC_INPUT1_PIN_ID	PIN0_ID
#define DC_INPUT2_PIN_ID	PIN1_ID


typedef enum {
	CW,ACW,STOP
}DcMotor_State;

void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
