/*
 * timer1.c
 *
 *  Created on: Nov 1, 2023
 *      Author: Dell-G5
 */

#include"timer1.h"
#include<avr/interrupt.h>

void (*timer1_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_OVF_vect){
	if(timer1_callBackPtr != NULL_PTR)
	{
		(*timer1_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect){
	if(timer1_callBackPtr != NULL_PTR)
	{
		(*timer1_callBackPtr)();
	}
}

void Timer1_init(const Timer1_ConfigType *Config_Ptr) {
	TCNT1 = Config_Ptr->initial_value;
	if (Config_Ptr->mode == CTC_OCR1A) {
		OCR1A = Config_Ptr->compare_value;
	}
	TCCR1A = (1 << FOC1A) | (1 << FOC1B)|(Config_Ptr->mode&0x03);
	TCCR1B = ((Config_Ptr->mode >> 2) << WGM12);
	TCCR1B = (TCCR1B & 0xF8)|(Config_Ptr->prescaler & 0x07);
	if (Config_Ptr->mode == CTC_OCR1A) {
		TIMSK |= (1 << OCIE1A);
	}
	if (Config_Ptr->mode == NORMAL_MODE) {
		TIMSK |= (1 << TOIE1);
	}
}

void Timer1_deInit(void) {
	TCCR1A = 0;
	TCCR1B = 0;
}

void Timer1_setCallBack(void (*a_ptr)(void)) {
	timer1_callBackPtr = a_ptr;
}




