/*
 * HMI_ECU.c
 *
 *  Created on: Nov 4, 2023
 *      Author: Dell-G5
 */
#include"HAL/keypad.h"
#include"HAL/lcd.h"
#include"MCAL/timer1.h"
#include"MCAL/uart.h"
#include<avr/io.h>
#include<util/delay.h>
#include"MCAL/gpio.h"

#define PASSWORD_LENGTH 5
#define IS_PASSWORD_SET 		'0'
#define SEND_FIRST_PASSWORD 	'1'
#define SEND_SECOND_PASSWORD 	'2'
#define ARE_THEY_MATCHED 		'3'
#define MATCHED 				'4'
#define UNMATCHED 				'5'
#define SEND_MAIN_PASSWORD 		'6'
#define CORRECT_PASSWORD 		'7'
#define WRONG_PASSWORD 			'8'
#define ALARM_MODE 				'9'
#define OPEN_DOOR 				'a'
#define SETTED 					'b'
#define NOT_SETTED 				'c'

uint8 g_timerticks=0;
uint8 password[5];

void timer1(void){
	g_timerticks++;
}

void enterPass(void){
	for(int i = 0 ; i < PASSWORD_LENGTH ; i++){
		password[i] = KEYPAD_getPressedKey();
		if(password[i] >= 0 && password[i] <= 9){
			LCD_intgerToString(password[i]);
			_delay_ms(500);
			LCD_moveCursor(1,i);
			LCD_displayCharacter('*');
		}
		else{
			i--;
		}
	}
	while(KEYPAD_getPressedKey() != '=');
}
void sendPass(uint8* pass){
	for(int i = 0 ; i < PASSWORD_LENGTH ; i++){
		UART_sendByte(pass[i]);
	}
}
int main(void){
	SREG |= (1<<7);
	Timer1_ConfigType Timer1Config = {0 , 31250 , CTC_OCR1A , FCPU_256};
	UART_ConfigType UART_MyConfig = {_8_BIT , PARITY_DISABLED , STOP_1_BIT , 9600};

	LCD_init();
	UART_init(&UART_MyConfig);
	Timer1_setCallBack(timer1);

	uint8 isPassSet=0;
	uint8 no_Tries=0;
	uint8 isPassCorrect=0;
	uint8 isPassMatch=0;
	LCD_displayStringRowColumn(0,0,"   DOOR LOCKER   ");
	_delay_ms(3000);
	UART_sendByte(IS_PASSWORD_SET);

	if(UART_recieveByte()==SETTED){
		isPassSet=1;
	}
	while(1){
		if(isPassSet){
			no_Tries=0;
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"+ : Open Door");
			LCD_displayStringRowColumn(1,0,"- : Change Pass");
			uint8 menuChoice = KEYPAD_getPressedKey();
			if(menuChoice=='+'){
				do {
					isPassCorrect=0;
					LCD_clearScreen();
					LCD_displayString("plz Enter Pass:");
					LCD_moveCursor(1,0);

					UART_sendByte(SEND_MAIN_PASSWORD);
					enterPass();
					sendPass(password);
					_delay_ms(15);
					if (UART_recieveByte()==CORRECT_PASSWORD) {

						UART_sendByte(OPEN_DOOR);
						g_timerticks = 0;
						Timer1_init(&Timer1Config);

						LCD_clearScreen();
						LCD_displayStringRowColumn(0,0,"Unlocking ");
						while(g_timerticks != 15);

						Timer1_deInit();
						g_timerticks = 0;
						Timer1_init(&Timer1Config);
						LCD_displayStringRowColumn(0,0,"Holding    ");
						while(g_timerticks != 3);

						Timer1_deInit();
						g_timerticks = 0;
						Timer1_init(&Timer1Config);
						LCD_displayStringRowColumn(0,0,"Locking    ");
						while(g_timerticks != 15);

						Timer1_deInit();
						isPassCorrect=1;
					}
					else {
						no_Tries++;

						LCD_clearScreen();
						LCD_displayString("Wrong Password!");
						_delay_ms(2000);

					}

				}
				while ((no_Tries < 3) && !isPassCorrect);
				if(isPassCorrect == 0){

					UART_sendByte(ALARM_MODE);
					LCD_clearScreen();
					g_timerticks = 0;
					Timer1_init(&Timer1Config);
					LCD_displayString("    WRONG!!!   ");
					LCD_displayStringRowColumn(1,0,"Retry After 1min");
					while(g_timerticks != 60);

					Timer1_deInit();
				}
			}
			else if(menuChoice=='-'){
				do{
					isPassCorrect = 0;
					LCD_clearScreen();
					LCD_displayString("plz Enter Pass:");
					LCD_moveCursor(1,0);
					isPassMatch = 1;
					UART_sendByte(SEND_MAIN_PASSWORD);
					enterPass();
					sendPass(password);
					_delay_ms(15);
					if(UART_recieveByte() == CORRECT_PASSWORD){
						isPassSet = 0;
						isPassCorrect = 1;
					}

					else{
						no_Tries++;
						LCD_clearScreen();
						LCD_displayString("Wrong Password!");
						_delay_ms(2000);

					}
				}
				while((no_Tries < 3) && !isPassCorrect);

				if(isPassCorrect == 0){
					UART_sendByte(ALARM_MODE);
					LCD_clearScreen();
					g_timerticks = 0;
					Timer1_init(&Timer1Config);

					LCD_displayString("    WRONG!!!   ");
					LCD_displayStringRowColumn(1,0,"Retry After 1min");
					while(g_timerticks != 60);

					Timer1_deInit();
				}
			}

		}
		else {
			LCD_clearScreen();
			LCD_displayString("Enter New Pass:");
			LCD_moveCursor(1,0);
			isPassMatch = 1;
			UART_sendByte(SEND_FIRST_PASSWORD);
			enterPass();
			sendPass(password);

			LCD_clearScreen();
			LCD_displayString("Re-Enter Pass:");
			LCD_moveCursor(1,0);
			UART_sendByte(SEND_SECOND_PASSWORD);
			enterPass();
			sendPass(password);

			UART_sendByte(ARE_THEY_MATCHED);
			isPassMatch = UART_recieveByte();

			if(isPassMatch == MATCHED){
				isPassSet = 1;
				isPassMatch = 0;
			}
			else {
				LCD_clearScreen();
				LCD_displayString("Not Same Pass");
				_delay_ms(1000);
			}
		}
	}
}

