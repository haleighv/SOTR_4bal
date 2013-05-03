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

//-----------------Function Prototypes-------------------//
//Function used to configure IOs
void vIO_init(void);

#endif /* INIT_H_ */