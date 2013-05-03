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
