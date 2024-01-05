#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char secs=0;
unsigned char mins=0;
unsigned char hrs=0;

unsigned char SW_T_flag=0;
unsigned char SEC_flag=0;
unsigned char MIN_flag=0;
unsigned char HRS_flag=0;

void INT0_init(void){
	DDRD &=~(1<<2);
	PORTD |= (1<<2);
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
}
void INT1_init(void){
	DDRD &=~(1<<3);
	MCUCR |= (1<<ISC10) | (1<<ISC11);
	GICR |= (1<<INT1);
}
void INT2_init(void){
	DDRB &=~(1<<2);
	PORTB |= (1<<2);
	MCUCSR &=~(1<<ISC2);
	GICR |= (1<<INT2);
}
void TIMER1_CMP_init(void){
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12)|(1<<CS12);
	TCNT1 = 0;
	OCR1A = 3906;
	TIMSK = (1<<OCIE1A);
}

ISR(INT0_vect){
	secs=0;
	mins=0;
	hrs=0;
	TCNT1=0;
}

ISR(INT1_vect){
	TCCR1B &=~(1<<CS12);
}

ISR(INT2_vect){
	TCCR1B |=(1<<CS12);
}

ISR(TIMER1_COMPA_vect){
	if(SW_T_flag==0){
		secs++;
		if(secs==60){
			secs=0;
			mins++;
		}
		if(mins==60){
			mins=0;
			hrs++;
		}
		if(hrs==100){
			hrs=0;
		}
	}
	else {
		secs--;
		if (secs == 255) {
			secs = 59;
			mins--;
			if (mins == 255) {
				mins = 59;
				hrs--;
				if (hrs == 255) {
					hrs = 0;
					mins = 0;
					secs = 0;
					TCCR1B &= ~(1<<CS12);
					PORTA |=(0x3F);
					PORTB |= (1<<3);
					_delay_ms(2000);
					PORTB &= ~(1<<3);
				}
			}
		}

	}
}

int main(void){
	SREG |= (1<<7);
	DDRC |= 0x0F;
	PORTC &=~(0x0F);
	DDRA |= 0x3F;
	PORTA &=~(0x3F);
	INT0_init();
	INT1_init();
	INT2_init();
	TIMER1_CMP_init();

	DDRD &=~(1<<0);
	DDRD &=~(1<<1);
	DDRD &=~(1<<4);
	DDRD &=~(1<<5);
	DDRD &=~(1<<6);
	DDRB |= (1<<3);
	PORTB &=~(1<<3);

	while(1){
		if((PIND & 0x01)==0x01){
			SW_T_flag=1;
		}
		else {
			SW_T_flag=0;
		}
		if(!(PIND & 0x10)){
			if(SEC_flag == 0){
				SEC_flag=1;
				if(!(PIND & 0x02)){
					secs+=5;
				}
				else {
					secs-=5;
				}
				if(secs>=60){
					secs=0;
				}
			}
		}
		else SEC_flag =0;

		if(!(PIND & 0x20)){

			if(MIN_flag==0){
				MIN_flag=1;
				if(!(PIND & 0x02)){
					mins+=1;
				}
				else {
					mins-=1;
				}
				if(mins>=60){
					mins=0;
				}
			}
		}
		else MIN_flag=0;

		if(!(PIND & 0x40)){
			if(HRS_flag==0){
				HRS_flag=1;

				if(!(PIND & 0x02)){
					hrs+=1;
				}
				else {
					hrs-=1;
				}
				if(hrs>=99){
					hrs=0;
				}
			}
		}
		else HRS_flag=0;

		PORTA = 0x01;
		PORTC = secs%10;
		_delay_ms(2);
		PORTA = 0x02;
		PORTC = secs/10;
		_delay_ms(2);
		PORTA = 0x04;
		PORTC = mins%10;
		_delay_ms(2);
		PORTA = 0x08;
		PORTC = mins/10;
		_delay_ms(2);
		PORTA = 0x10;
		PORTC = hrs%10;
		_delay_ms(2);
		PORTA = 0x20;
		PORTC = hrs/10;
		_delay_ms(2);
	}
}
