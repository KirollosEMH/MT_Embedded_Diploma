 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ATmega16 ADC driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "avr/io.h" /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType * Config_Ptr)
{
	ADMUX = ((Config_Ptr->ref_volt & 0x03)<<6);
	SET_BIT(ADCSRA,ADEN);
	ADCSRA = (ADCSRA& 0xF8) | (Config_Ptr->prescaler & 0x07);

}

uint16 ADC_readChannel(uint8 channel_num)
{
	ADMUX = (ADMUX& 0xE0) | (channel_num & 0x07);
	SET_BIT(ADCSRA,ADSC);
	while(BIT_IS_CLEAR(ADCSRA,ADIF));
	SET_BIT(ADCSRA,ADIF);
	return ADC;
}
