#include "init.h"




void init()
{


	SMCR &= ~(0<<SE);
	MCUCR |= (1<<PUD);
	PCICR |= (1<<PCIE2);						// enable interrrupt PCINT[16:23]
	PCMSK2 |= _BV(PCINT22);

	DDRB = 0;
	PORTB = 0;
	DDRB |= (1<<PB1);

	
	DDRD = 0;
	DDRD |= (1<<PD3);
	PORTD &= ~_BV(PD3);							// Triac OFF
	PORTB &= ~_BV(PB1);							// LED OFF
	
	ADC_Init();
	TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);	// Timer prescale F_CPU 256 and start timer
	TIMSK1 |= (1<<ICIE1);
//	AComp_Init();
	



}

void ADC_Init(){
 ADCSRA |= (1 << ADPS2)|(1 << ADPS0)|(0 << ADIE)|(1<<ADEN)|(1<<ADSC);    
 ADMUX |= (0 << REFS1)|(1 << REFS0)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3)|(1<<ADLAR);
 
}

/*
void AComp_Init()
{
  
  ACSR |= (1<<ACD)|(0<<ACBG)|(0<<ACI)|(1<<ACIE)|(0<<ACIC )|(1<<ACIS1)|(0<<ACIS0);
  DIDR1 |= (1<<AIN1D)|(1<<AIN0D);
  ADCSRB|=(0<<ACME)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
  
}


*/
