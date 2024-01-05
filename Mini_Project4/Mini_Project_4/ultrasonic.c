/*
 * ultrasonic.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Dell-G5
 */
#include"ultrasonic.h"
#include"icu.h"
#include"gpio.h"
#include <util/delay.h>


volatile uint8 g_edge_flag;

void Ultrasonic_init(void){
	ICU_ConfigType icuConfig;
	icuConfig.clock = F_CPU_8;
	icuConfig.edge = RAISING;

	ICU_init(&icuConfig);
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}

void Ultrasonic_Trigger(void){
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void){
	uint16 distance;
	Ultrasonic_Trigger();
	while(g_edge_flag != 2);
	distance = (uint16)((float32)(ICU_getInputCaptureValue()/58.8));
	g_edge_flag=0;
	return distance;
}

void Ultrasonic_edgeProcessing(void){

	g_edge_flag++;
	if(g_edge_flag==1){
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(g_edge_flag==2){
		ICU_setEdgeDetectionType(RAISING);

	}

}
