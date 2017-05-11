#ifndef _STD_LIB_INC_
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#endif




#ifndef _INIT_H_
#include "init.h"
#endif

#ifndef _MEASUREMENTS_H_
#include "measurements.h"
#endif

void blink_after_working(void);

uint8_t is_Button_Push(void);

volatile uint16_t timeT1 = 0;												// t1-t2
volatile uint16_t timeT2 = 0;												// t2-t3
volatile uint16_t deltaTime1 = 0;											// t2-0'
volatile uint16_t deltaTime2 =  0;											// t1-0"


