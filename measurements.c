#include "measurements.h"


uint16_t measureADC()
{

	ADCSRA |= _BV(ADSC);
	while ((ADCSRA & (1 << ADIF)) == 0)    // 1<< ADIF = 0b00010000
	 {
		ADCSRA |= (1 << ADSC);
	 }
	return ADCH;
//	return (ADCL|ADCH << 8);
}

uint16_t clearADC()
{
	int i;
	uint16_t temp[8] ;
	for(i=0; i<=7;i++)
	{
		temp[i]= measureADC();
	}
//	qsort(temp, 5, sizeof(int), cmp );
	for(i=1; i<=7;i++)
	{
		temp[0]+=temp[i];
	}
	return temp[0]/8;	
}

uint16_t  cmp(const void *a, const void *b) 
{
     return *(int*)a - *(int*)b;
}

uint16_t measureNet(void)
{
	TCNT1 = 0x0;
	measure_wait = 1;
	sei();
	

	while(measure_wait)
	{
		if(signal_label == 0x01)
		 {
			measure_wait = 0;
			calcTime.timeT1 = TCNT1;							// Capture rising time
		}

	}

	measure_wait = 1;
	while(measure_wait)
	{
		if(signal_label == 0x10)
		 {
			measure_wait = 0;
			calcTime.timeT2 = TCNT1;							// Capture falling time
		 }
	}

	
	measure_wait = 1;
	while(measure_wait)
	{
		if(signal_label == 0x01)
		 {
			measure_wait = 0;
			calcTime.timeT3 = TCNT1;							// Capture negative half-wave time
			cli();
		 }
	}
	
	return 0;


}

uint16_t clearNet(void)
{
	int16_t timeT[2][3];
	for(int i =0; i<=1;i++)
	{
		measureNet();
		timeT[i][0] = calcTime.timeT1;
		timeT[i][1] = calcTime.timeT2;
		timeT[i][2] = calcTime.timeT3;
	}
	
	if( ( abs(timeT[0][0]-timeT[1][0]) <20)& 
		( abs(timeT[0][1]-timeT[1][1]) <20)& 
		( abs(timeT[0][2]-timeT[1][2]) <20) )
	{
/*		calcTime.timeT1 = timeT[2][0];
		calcTime.timeT2 = timeT[2][1];
		calcTime.timeT3 = timeT[2][2];*/

		return 1;
	}
	else
	 {
		clearNet();
		return 0;
	 }
}



uint8_t wait_Wave(uint16_t deltaTime2)
{
	sei();
	measure_wait = 1;
	
	while(measure_wait)
	{
		if(signal_label == 0x01)
		 {
		 	cli();
			TCNT1 = 0x0;
			measure_wait = 0;
		
		 }

	}

	measure_wait = 1;
	while(measure_wait)
	 {
		if(TCNT1>=deltaTime2)
			return 0;
	 }
	return 1;
}




