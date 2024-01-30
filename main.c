/*
 * POV.c
 *
 * Created: 10/28/2022 1:20:00 PM
 * Author : Hos
 */ 
 #include "MCAL\micros\micros.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>

#define READ_BIT(x,BIT_NO) (x& (1<<BIT_NO)) >> BIT_NO 

unsigned long overflows_count=0;

#define true 1
#define false 0
uint8_t image[16][16] = {{true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
{true,true,true,true,true,true,true,true,true,true,true,true,true,true,false,false},
{true,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true},
{true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,true},
{true,true,true,true,true,true,true,true,true,true,true,false,false,true,true,true},
{true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,true},
{true,true,true,true,true,true,true,true,true,false,true,true,true,true,true,true},
{true,true,true,true,true,true,true,true,false,true,true,true,true,true,true,true},
{true,true,true,true,true,true,true,false,true,true,true,true,true,true,true,true},
{true,true,true,true,true,true,false,true,true,true,true,true,true,true,true,true},
{true,true,true,true,false,false,true,true,true,true,true,true,true,true,true,true},
{true,true,true,false,false,true,true,true,true,true,true,true,true,true,true,true},
{true,true,false,false,true,true,true,true,true,true,true,true,true,true,true,true},
{true,false,false,true,true,true,true,true,true,true,true,true,true,true,true,true},
{false,false,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
{true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true}};
	
	uint8_t image1[16][16] = {{true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
		{true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false},
		{true,true,false,true,true,true,true,true,true,true,true,true,true,false,false,true},
		{true,true,true,false,true,true,true,true,true,true,true,true,false,false,true,true},
		{true,true,true,true,false,true,true,true,true,true,true,false,true,true,true,true},
		{true,true,true,true,true,false,true,true,true,true,false,true,true,true,true,true},
		{true,true,true,true,true,true,false,true,true,false,true,true,true,true,true,true},
		{true,true,true,true,true,true,true,false,false,true,true,true,true,true,true,true},
		{true,true,true,true,true,true,true,false,false,true,true,true,true,true,true,true},
		{true,true,true,true,true,true,false,true,true,false,true,true,true,true,true,true},
		{true,true,true,true,false,false,true,true,true,true,false,true,true,true,true,true},
		{true,true,true,false,false,true,true,true,true,true,false,false,true,true,true,true},
		{true,true,false,false,true,true,true,true,true,true,true,false,false,true,true,true},
		{true,false,false,true,true,true,true,true,true,true,true,true,false,false,true,true},
		{false,false,true,true,true,true,true,true,true,true,true,true,true,false,false,true},
		{true,true,true,true,true,true,true,true,true,true,true,true,true,true,false,true}};
			
	unsigned long intrrupt_time=0,counter1=0, angle_0_t , current_angle_t,required_angle_t ;
uint8_t prev_state=0;
float full_rotation=0;
float one_deg=0;
uint8_t text_ok =0;


void delayMicroseconds(unsigned int us)
{
	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
	return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;

	// busy wait
	__asm__ __volatile__ (
	"1: sbiw %0,1" "\n\t" // 2 cycles
	"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void wait_us(int us)
{
	for( int i=0; i < us; i++)
	{
		_delay_us(1);
	}
}
uint8_t A[]={0b11111111,0b10010000,0b00111000,0b10010000,0b11111111};
uint8_t B[]={0b11111111,0b10010001,0b10010001,0b10010001,0b01101110};
uint8_t C[]={0b00111100,0b01000010,0b10000001,0b10000001,0b10000001};
uint8_t D[]={0b11111111,0b10000001,0b10000001,0b01000010,0b00111100};
uint8_t E[]={0b11111111,0b10010001,0b10010001,0b10010001,0b10010001};
uint8_t F[]={0b11111111,0b10010000,0b10010000,0b10010000,0b10010000};
uint8_t G[]={0b01111111,0b10000001,0b10001001,0b10001001,0b10001110};
uint8_t H[]={0b11111111,0b00001000,0b00001000,0b00001000,0b11111111};
uint8_t I[]={0b10000001,0b10000001,0b11111111,0b10000001,0b10000001};
uint8_t J[]={0b00000110,0b00001001,0b00000001,0b00000001,0b11111110};
uint8_t K[]={0b11111111,0b00011000,0b00100100,0b01000010,0b10000001};
uint8_t L[]={0b11111111,0b00000001,0b00000001,0b00000001,0b00000001};
uint8_t M[]={0b11111111,0b01000000,0b00100000,0b01000000,0b11111111};
uint8_t N[]={0b11111111,0b00100000,0b00011000,0b00000100,0b11111111};
uint8_t O[]={0b01111110,0b10000001,0b10000001,0b10000001,0b01111110};
uint8_t P[]={0b11111111,0b10010000,0b10010000,0b10010000,0b01100000};
uint8_t Q[]={0b01111110,0b10000001,0b10000101,0b01111110,0b00000001};
uint8_t R[]={0b11111111,0b10011000,0b10010100,0b10010010,0b01100001};
uint8_t S[]={0b01110001,0b10001001,0b10001001,0b10001001,0b10001110};
uint8_t T[]={0b10000000,0b10000000,0b11111111,0b10000000,0b10000000};
uint8_t U[]={0b11111110,0b00000001,0b00000001,0b00000001,0b11111110};
uint8_t V[]={0b11111100,0b00000010,0b00000001,0b00000010,0b11111100};
uint8_t W[]={0b11111111,0b00000010,0b00000100,0b00000010,0b11111111};
uint8_t X[]={0b11000011,0b00100100,0b00011000,0b00100100,0b11000011};
uint8_t Y[]={0b11000000,0b00100000,0b00011111,0b00100000,0b11000000};
uint8_t Z[]={0b10000111,0b10001001,0b10010001,0b10100001,0b11000001};
uint8_t blank[]={0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};




void draw_a_char(uint8_t* c, float  de) // array name is a pointer
{
	 
	for (int n=0; n<=sizeof(c); n++)
	{                 // every n represent a row in the char 
		for (uint8_t Pin = 1; Pin<=7; Pin++)
		 {   // even Pin represent a led in the row 
			if ( READ_BIT(*(c+n),Pin) ==0)                // read lsb first
			{
				PORTA &= ~(1<<(7-Pin));
			}
			else
			{
				PORTA |= (1<<(7-Pin));
			}
		}
	delayMicroseconds(de);             // wait between a row and the next
	}
}

void draw_a_string(char* x,int  de)
{
	
	for (uint8_t i=0 ; i<= strlen(x) ; i++)
	{
		
		switch (x[i])
		{
			case 'A':
			draw_a_char(A,de);
			break;
			
			case 'B':
			draw_a_char(B,de);
			break;
			
			case 'C':
			draw_a_char(C,de);
			break;
			
			case 'D':
			draw_a_char(D,de);
			break;
			
			case 'E':
			draw_a_char(E,de);
			break;
			
			case 'F':
			draw_a_char(F,de);
			break;
			
			case 'G':
			draw_a_char(G,de);
			break;
			
			case 'H':
			draw_a_char(H,de);
			break;
			
			case 'I':
			draw_a_char(I,de);
			break;
			
			case 'J':
			draw_a_char(J,de);
			break;
			
			case 'K':
			draw_a_char(K,de);
			break;
			
			case 'L':
			draw_a_char(L,de);
			break;
			
			case 'M':
			draw_a_char(M,de);
			break;
			
			case 'N':
			draw_a_char(N,de);
			break;
			
			case 'O':
			draw_a_char(O,de);
			break;
			
			case 'P':
			draw_a_char(P,de);
			break;
			
			case 'Q':
			draw_a_char(Q,de);
			break;
			
			case 'R':
			draw_a_char(R,de);
			break;
			
			case 'S':
			draw_a_char(S,de);
			break;
			
			case 'T':
			draw_a_char(T,de);
			break;
			
			case 'U':
			draw_a_char(U,de);
			break;
			
			case 'V':
			draw_a_char(V,de);
			break;
			
			case 'W':
			draw_a_char(W,de);
			break;
			
			case 'X':
			draw_a_char(X,de);
			break;
			
			case 'Y':
			draw_a_char(Y,de);
			break;
			
			case 'Z':
			draw_a_char(Z,de);	
			break;
			
			case ' ':
			draw_a_char(blank,de);
			break;	
		}
		
		draw_a_char(blank,de);
		
	}
}

uint8_t p_image[120] = {0,0,0,0,0,0,0,0,0,0,128,128,0,128,128,160,32,224,224,224,240,224,0,0,0,0,0,0,0,0,0,0,0,0,0,64,64,224,240,224,224,192,0,128,128,128,0,
128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,64,64,64,192,192,224,192,192,64,64,192,64,64,96,224,96,64,
192,192,224,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,224,192,192,64,96,224,96,64,64,192,64,64,192,192,224,192,192,64,64,64,0,0,0};


int i=0;
int j=0;
int b=0;
int r=0;
int t=0;
uint8_t flag=0;
float angle_x;


uint8_t arr[180];


int main(void)
{
	
	micros_init();
	DDRD = 0x00;
	DDRA = 0xFF;
	MCUCR = 1<<ISC01 | 1<<ISC00;          // interrupt with falling edge (one full rotation)
	GICR = (1<<INT0);
	sei(); 
	
    while (1) 
    {
		current_angle_t = micros();
		sei();
		required_angle_t = current_angle_t - angle_0_t;
		//angle = required_angle_t/one_deg;   // in degrees
		

			
		if ( (required_angle_t >=one_deg*120) && (required_angle_t < one_deg*121) && (text_ok) )		
				{
					for (int n=0; n<=sizeof(p_image); n++)
					{                 // every n represent a row in the char
						for (uint8_t Pin = 1; Pin<=7; Pin++)
						{   // even Pin represent a led in the row
							if ( READ_BIT(*(p_image+n),Pin) ==0)                // read lsb first
							{
								PORTA &= ~(1<<(7-Pin));
							}
							else
							{
								PORTA |= (1<<(7-Pin));
							}
						}
						delayMicroseconds(one_deg*3);             // wait between a row and the next
					}
		//draw_a_char(p_image,one_deg*3);
				}
				else
				{
					PORTA = 0x00;
				}

    
}
}

ISR(INT0_vect)
{

 	intrrupt_time =  micros();
	sei();
	if (PIND & 0b00000100)
	{
		if (prev_state == 0)           // this is the first rotation of the POV
		{
			prev_state=1;             // This is not the first anymore
			counter1 = intrrupt_time;
		}
		
		else       // The second full rotation interrupt will enter here
		{
			full_rotation = intrrupt_time - counter1;
			one_deg = full_rotation/360;
			prev_state=0;               // in order to calculate the next full rotation
 			angle_0_t=micros();
			sei();
				text_ok=1;
		}
		
	}
	
	
}

ISR(TIMER0_OVF_vect)
{
	overflows_count++;       // timer flag is cleared by hardware after executing the interrupt
}