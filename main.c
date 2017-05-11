
#include "main.h"
/*
Измеренное время задержки включения оптрона TLP181 составляет 375 мкс.
Измеренное время задержки включения оптрона MOC3052 и симистра BTA15-600 составляет 6мкс,
что составляет 0,18 тика таймера. Поэтому эту задержку принимаем равной 0.
*/
//#define F_CPU 8000000
#define OPTO_DELAY 150									// 0.375 ms = 12 ticks
#define TRIAC_DELAY 0									// 6us = 0.18 tick
#define OFF_COEF 63										// 2ms = 62.5 ticks


ISR(PCINT2_vect)
{
	if ((PIND & 0b01000000)==0)
	{
		// Rising edge
		signal_label = 0x01;

	}
	else
	{	
		// Falling edge
		signal_label = 0x10;
	}
}
 

int main(void)
{

init();

uint8_t buttonPush = 1;
uint16_t valueADC = 0;
uint16_t wave_length = 0;
uint16_t count_Wave = 0;
uint16_t work_Time = 0;



while(1)
{
//-----------------------------------------------------------------
//valueADC = measureADC();
valueADC = clearADC();

//measureNet();
clearNet();



buttonPush = is_Button_Push();






//-------------------------- manual mode -----------------------------------
//if((valueADC >=1013)&(buttonPush==0))
if((valueADC >=232)&(buttonPush==0))
{	
//	measureNet();
	timeT1 = calcTime.timeT2-calcTime.timeT1;				// Вычисление интервала t1-t2
	timeT2 = calcTime.timeT3-calcTime.timeT2;				// Вычисление интервала t2-t3
	deltaTime1 = (timeT2-timeT1)/4;							// Вычисление интервала t2-0'
	deltaTime2 =  timeT1 + timeT2 - deltaTime1;				// Вычисление интервала t1-0"
	deltaTime2 = deltaTime2-OPTO_DELAY-TRIAC_DELAY;

	wait_Wave(deltaTime2);

	PORTB |= _BV(PB1);											// LED ON
	PORTD |= _BV(PD3);											// Triac ON
	
	while(buttonPush==0)
	 {
	 	_delay_us(50);
		buttonPush = is_Button_Push();
	 }
	wait_Wave(deltaTime2-OFF_COEF);

	PORTD &= ~_BV(PD3);											// Triac OFF
	PORTB &= ~_BV(PB1);											// LED OFF
	blink_after_working();										// blink 
}
//-------------------------- end manual mode --------------------------------

//-------------------------- automatic mode ---------------------------------
else
{	
//	if((valueADC <1013)&(buttonPush==0))
	if((valueADC <232)&(buttonPush==0))						
	 {	
//		measureNet();
		timeT1 = calcTime.timeT2-calcTime.timeT1;				// t1-t2
		timeT2 = calcTime.timeT3-calcTime.timeT2;				// t2-t3
		deltaTime1 = (timeT2-timeT1)/4;							// t2-0'
		deltaTime2 =  timeT1 + timeT2 - deltaTime1;				// t1-0"
		deltaTime2 = deltaTime2-OPTO_DELAY-TRIAC_DELAY;
		wave_length = (2*timeT1) + (4*deltaTime1);

		count_Wave = 0;											// Calculation numbers of waves
//		if(valueADC<51)
		if(valueADC<10)
		 {
			count_Wave += 1;
		 }
//		if(valueADC>=51)
		if(valueADC>=10)
		 {	
//		 	count_Wave += 2+ (valueADC-50)/13;
			count_Wave += 2+ (valueADC-9)/3;
		 }	
//		count_Wave = 1;	
				
		wait_Wave(deltaTime2);

		PORTB |= _BV(PB1);									// LED ON
		PORTD |= _BV(PD3);									// Triac ON
		
		
		work_Time =  count_Wave * wave_length;
		TCNT1 = 0x0;
		while(TCNT1<(work_Time-TRIAC_DELAY-OFF_COEF))
		 {
			
		 }
			
		
 	 PORTD &= ~_BV(PD3);										// Triac OFF
	 PORTB &= ~_BV(PB1);										// LED OFF
	 blink_after_working();										// blink 	


	 }//-------------------end automatic mode

}//-----------------------end else

}//-----------------------end while(1)



return 0;
}//------------------------ end main

//------------------------------------------------------------- delay after working and blinking
void blink_after_working(void)									
{
																
			for(int i =0; i <5;i++)
			{
				PORTB |= _BV(PB1);
				_delay_ms(200);
				PORTB &= ~_BV(PB1);
				_delay_ms(200);	
			}

			PORTB &= ~_BV(PB1);
}
//------------------------------------------------------------- Is Button push?
uint8_t is_Button_Push(void)									
{
	uint8_t is_Button_Push = 1;
	_delay_ms(10);
	is_Button_Push = PINB & 0b00000001;
	if(is_Button_Push==0)
	 {
		_delay_ms(10);
		is_Button_Push = PINB & 0b00000001;
		if(is_Button_Push==0)
		 {
				_delay_ms(5);
				is_Button_Push = PINB & 0b00000001;
				if(is_Button_Push==0)
				 {
						return is_Button_Push;
				 }
		 }

	 }
	
	return 1;
}
