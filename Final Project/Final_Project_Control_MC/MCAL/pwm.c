/*
 * pwm.c
 *
 *  Created on: Oct 13, 2023
 *      Author: Kirollos Ehab
 */
#include"pwm.h"
#include"gpio.h"
#include<avr/io.h>


void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0 = 0; // Set Timer Initial Value to 0

	uint8 speed = (255);

	OCR0  = speed;

	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
