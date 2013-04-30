/*********************************************************
* File: init.c
*
* Description: This is a header file for functions used to
*			   initialize the STK600 IOs used in lab4.
*			   Port A is configured to outputs, and each
*			   pin is set to output high to initialize
*			   the active low LEDs attached to be off.
*
* Author: Haleigh Vierra, Matt Cruse
*
* Revisions:
*   4/29/2013 HAV Original File Created
*
**********************************************************/
//-------------------Library Includes--------------------//
#include "init.h"

//----------------Function Definitions-------------------//

/*-----------------------------------------------------
* Function: vIO_init
*
* Description: This function initializes all pins on PORTA
*              as outputs, and sets them all to '1' as to
*              initialize the attached LEDs to be off, and
*			   initializes PORTE as inputs for the switches
*
* return: void
*--------------------------------------------------------*/
void vIO_init(void)
{
	// Set PortA (LEDs) as all outputs
	DDRA = 0xFF;
	// Turn LEDs off
	PORTA = 0xFF;
}


/*-----------------------------------------------------
* Function: initialize_timer2
*
* Description: This function Initializes timer2 to
*			   generate interrupts to control LED blinking.
*			   Timer2 is configured to generate interrupts
*			   once the timer reaches the value stored in
*			   OCR2A
*
* return: void
*--------------------------------------------------------*/
void initialize_timer2()
{
	TIMSK2 = (1 << OCIE2A);//Set interrupt mask for OC2A
	TCCR2A = ((1 << COM2A1) | (1 << COM2A0)|(1<<WGM21));//Set OC2A on Compare Match
	TCCR2B = ((1 << CS22) |(1 << CS21) | (1 << CS20));//Set timer2 to CTC, prescaler of 1024
	OCR2A  = TIMER2_10Hz; //initialize ocra2 to value corresponding to 14Hz interrupt generation
}
