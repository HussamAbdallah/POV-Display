/*
 * micros.h
 *
 * Created: 10/28/2022 1:37:14 PM
 *  Author: Hos
 */ 


#ifndef MICROS_H_
#define MICROS_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long micros();
unsigned long millis();
void micros_init();




#endif /* MICROS_H_ */