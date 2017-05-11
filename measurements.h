#ifndef _STD_LIB_INC_
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <inttypes.h>
#endif



volatile uint8_t signal_label;
volatile uint8_t measure_wait;

uint16_t  cmp(const void *a, const void *b);

uint16_t clearADC(void);
uint16_t measureADC(void);


uint16_t measureNet(void);
uint16_t clearNet(void);

uint8_t is_Wave_begin(void);

uint8_t wait_Wave(uint16_t deltaTime2);

struct timer
{
	uint16_t timeT1;
	uint16_t timeT2;
	uint16_t timeT3;
} calcTime;
