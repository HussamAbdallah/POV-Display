/*
 * micros.c
 *
 * Created: 10/28/2022 1:37:03 PM
 *  Author: Hos
 */ 


#include "micros.h"

extern unsigned long overflows_count;

unsigned long micros() {
	unsigned long overflows;
	uint8_t oldSREG = SREG, t;
	
	cli();
	overflows = overflows_count;
	t = TCNT0;
	
	if ((TIFR & (1<<TOV0)) && (t < 255))          // current counts before overflow (before entering the ISR)
	overflows++;
	
	SREG = oldSREG;
	
	unsigned long result = ( (overflows*256) + t) *4;
	
	return result;
}

void micros_init()
{
	TCCR0 = 0x00;				// Normal mode
	TCNT0=0;					// timer starting value
	TCCR0 = 0b00000011;		   // start timer by setting prescaler to 64
	TIMSK |= (1<<TOIE0);	   // enable TIMER0 overflow interrupt
	sei();                     // enable global interrupts
	
}