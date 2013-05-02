/*********************************************************
* File: init.h
*
* Description: This is a header file for functions used to
*			   initialize the STK600 IOs used in lab4.
*
* Author: Haleigh Vierra, Matt Cruse
*
* Revisions:
*   4/29/2013 HAV Original File Created
*
**********************************************************/
#ifndef INIT_H_
#define INIT_H_

//-------------------Library Includes--------------------//
#include <avr/io.h>

//OCR_value = clk_freq / prescaler / desired_frequency / #clk_edges
#define TIMER2_10Hz 781 //Value for OCR2A corresponding to 10Hz

//-----------------Function Prototypes-------------------//
//Function used to configure IOs
void vIO_init(void);

// Function used to configure timer interrupt
void initialize_timer2(void);

#endif /* INIT_H_ */