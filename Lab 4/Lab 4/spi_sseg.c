/* ----------------------------------------------------------------
 * Filename: spi_sseg.c
 * 
 *
 * Author: Haleigh Vierra, Matt Cruse
 *
 * Revisions:
 *   4/29/2013 HAV Original File Created
 *-----------------------------------------------------------------
 */ 
#include "spi_sseg.h"


//--------------------------------------------------------------
//- Function: SPI_MasterInit()
//- 
//- return value: none
//- parameters: none
//-
//- Description: Initializes the AVR device as a master and 
//- and turns on the device and sets the clock frequency. 
//- For convenience, this function also configures the SPI
//- port GPIOs
//- The seven segment display is configured so clock line 
//- should idle low and data is sampled on the rising edge
//-
//-
//---------------------------------------------------------------
void SPI_MasterInit(void)
{
	//SPCR |= ((1 << SPE) | (1 << SPIE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0)); 
	//SPSR |=  1 << SPI2X; 
	
	DDRB = ((1 << SPI_SS) | (1 << SPI_MOSI) | (1 << SPI_SCK)); 
	//make sure SS is high
	PORTB |= (1 << SPI_SS);

	SPCR =  (0<<SPIE) |	//No interrupts
		    (1<<SPE)  |	//SPI enabled
			(0<<DORD) |	//shifted out MSB
			(1<<MSTR) |	//master
			(0<<CPOL) |	//rising leading edge
			(0<<CPHA) |	//sample leading edge
			(1<<SPR1) | //
			(1<<SPR0) ; //clock speed
	
	SPSR =  (0<<SPIF) |	//SPI interrupt flag
			(0<<WCOL) |	//Write collision flag
			(1<<SPI2X);	//Doubles SPI clock
			
	
	SSEG_Set_Brightness(10);
}

//--------------------------------------------------------------
//- Function: SPI_MasterTransmit()
//- 
//- return value: none
//- parameters: data to be transmitted
//-
//- Description: This function selects the device, transmits
//- the data, waits for a the transmission to complete with 
//- a poll, then deselects the device. 
//---------------------------------------------------------------
void SPI_MasterTransmit(uint8_t data)
{
	//xSemaphoreTake( xMutex, portMAX_DELAY);
	
	//set SS low
	PORTB &= ~(1 << SPI_SS);
	//Load data to be transferred
	SPDR = data;
	//wait for spi interrupt flag to signal complete transfer
	while (!(SPSR & (1 << SPIF))); 
	//Set SS high 
	PORTB |= (1 << SPI_SS);	
	
	//xSemaphoreGive( xMutex );
}



//--------------------------------------------------------------
//- Function: SSEG_Set_Brightness()
//- 
//- return value: none
//- parameters: val, the value to set the brightness of display
//-
//- Description: This function set the brightness of the 
//- display where 255 is off and 0 is on as bright as possible. 
//---------------------------------------------------------------
void SSEG_Set_Brightness(uint8_t val)
{
	SPI_MasterTransmit(SSEG_BRIGHTNESS);	
	SPI_MasterTransmit(val);
}



//--------------------------------------------------------------
//- Function: SSEG_Reset()
//- 
//- return value: none
//- parameters: none
//-
//- Description: This function turns off all display elements. 
//---------------------------------------------------------------
void SSEG_Reset(void)
{
	SPI_MasterTransmit(SSEG_RESET);	
}



//--------------------------------------------------------------
//- Function: SSEG_Write_4vals_array()
//- 
//- return value: none
//- parameters: pointer to an array
//-
//- Description: This function takes a pointer that in theory is 
//- pointing to an array of four byte values, one for 
//- each of the 7-segment display locations. The four values
//- are then written to the 7-segment display.  
//---------------------------------------------------------------
void SSEG_Write_4vals_array(uint8_t* vals)
{
	uint8_t i;
	for(i = 0; i < 4; i ++)
	{
		SPI_MasterTransmit(*(vals + i));			
	}		
}



//--------------------------------------------------------------
//- Function: SSEG_Write_digit()
//- 
//- return value: none
//- parameters: digit location and digit value
//-
//- Description: This is a wrapper function for writing 
//- individual digits to the display. In this case, you need 
//- to first send the initial escape character before you 
//- send the actual data. 
//---------------------------------------------------------------
void SSEG_Write_digit(uint8_t digit, uint8_t val)
{
	switch(digit)
	{
		case 1:
			SPI_MasterTransmit(DIGIT_1);
			SPI_MasterTransmit(val);
			break; 
		
		case 2:
			SPI_MasterTransmit(DIGIT_2);
			SPI_MasterTransmit(val);
			break;
		
		case 3:
			SPI_MasterTransmit(DIGIT_3);
			SPI_MasterTransmit(val);
			break;
		
		case 4:
			SPI_MasterTransmit(DIGIT_4);
			SPI_MasterTransmit(val);
			break;
			
		default:	
			break;
	}	
}



//--------------------------------------------------------------
//- Function: SSEG_Write_left_digits()
//- 
//- return value: none
//- parameters: binary value for display on two left-most digits
//-
//- Description: This function decomposes the sent value into
//- a tens and ones digit the then sends them off to the two 
//- left-most digits of the display. This function also handles
//- lead zero blanking. 
//---------------------------------------------------------------
void SSEG_Write_left_digits(uint8_t val)
{
	//variable to store the 1's place value once it's determined
	uint8_t dig_ones_place = val;
	//variable to store the 10's place value once it's determined
	uint8_t dig_tens_place = 0; 
	
	//2 digits of 7seg can display up to 99. Check bounds of 'val'
	if(val < 100)
	{
		dig_tens_place = val / 10;
		dig_ones_place = val % 10;
		
		//SPI_MasterTransmit(DIGIT_1);
		SPI_MasterTransmit(dig_tens_place);
		//SPI_MasterTransmit(DIGIT_2);
		SPI_MasterTransmit(dig_ones_place);
	}		
}



//--------------------------------------------------------------
//- Function: SSEG_Write_right_digits()
//- 
//- return value: none
//- parameters: binary value for display on two right-most digits
//-
//- Description: This function decomposes the sent value into
//- a tens and ones digit the then sends them off to the two 
//- right-most digits of the display. This function also handles
//- lead zero blanking. 
//---------------------------------------------------------------
void SSEG_Write_right_digits(uint8_t val)
{
	//variable to store the 1's place value once it's determined
	uint8_t dig_ones_place = val;
	//variable to store the 10's place value once it's determined
	uint8_t dig_tens_place = 0;
		
	//2 digits of 7seg can display up to 99. Check bounds of 'val'
	if(val < 100)
	{
		//subtract from tens place until it's zero to determine
		//tens and ones place digits
		while((dig_ones_place - 10) >= 0)
		{
			dig_ones_place -= 10;
			dig_tens_place++;
		}
		//SPI_MasterTransmit(DIGIT_3);
		SPI_MasterTransmit(dig_tens_place);
		//SPI_MasterTransmit(DIGIT_4);
		SPI_MasterTransmit(dig_ones_place);
	}	
}


//--------------------------------------------------------------
//- Function: SSEG_Write_Decimal_Point()
//- 
//- return value: none
//- parameters: the individual values for the various "dots" on 
//-             the 7-segment display. 
//-
//- Description: Writes decimal point values to the display. 
//- There are six different values that can be written. Check 
//- out the 7-segment device spec for details on decimal points.
//---------------------------------------------------------------
void SSEG_Write_Decimal_Point(uint8_t val)
{
	SPI_MasterTransmit(SSEG_DEC_PNT);
	SPI_MasterTransmit(val);
}


