/*********************************************************
* File: lab2_main.h
*
* Description: This is a main function that creates tasks
*              using FreeRTOS to:
*				• Blink LED0 at approximately 1 Hz
*				• Blink LED2 at approximately 2 Hz
*				• Blink LED4 at approximately 4 Hz
*				• Blink LED6 at approximately 8 Hz
*              
*               LED0, LED2, LED4, and LED6 are connected 
*               to PORTB pin 0, 2, 4, and 6 respectively. 
*
* Author: Haleigh Vierra, Matt Cruse
*
* Revisions:
*   4/16/2013 HAV Original File Created
*
**********************************************************/
//-------------------Library Includes--------------------//
#include <stdint.h>
#include <avr/io.h>

#include "FreeRTOS.h"
#include "task.h"
#include "init.h"

//----------------------Definitions----------------------//
#define F_CPU 16000000
#define DELAY_MS_1HZ 500
#define DELAY_MS_2HZ 250
#define DELAY_MS_4HZ 125
#define DELAY_MS_8HZ 63

//-----------------Function Prototypes-------------------//
void vTaskFunction_LEDToggle(void *pvLED_blink_specs); 

//-------------------Main Function-----------------------//
int main( void )
{  	
    //variable definitions: arrays; element 1 is the LED
	//						position in PORTB, element 2
	//                      is the number of milliseconds
	//                      delay needed for each blink
	//                      frequency
	uint16_t LED0[2] = {0, DELAY_MS_1HZ}; 
	uint16_t LED2[2] = {2, DELAY_MS_2HZ}; 
	uint16_t LED4[2] = {4, DELAY_MS_4HZ}; 
	uint16_t LED6[2] = {6, DELAY_MS_8HZ}; 
	
    // Function initializing PORTB as an output and setting LEDs low
	vIO_init(); 


	// Four instances of the same task function created to blink the 4 LEDs
	xTaskCreate(vTaskFunction_LEDToggle, "LED0 Task", configMINIMAL_STACK_SIZE, (void *) LED0, 1, NULL );
	xTaskCreate(vTaskFunction_LEDToggle, "LED2 Task", configMINIMAL_STACK_SIZE, (void *) LED2, 2, NULL );
	xTaskCreate(vTaskFunction_LEDToggle, "LED4 Task", configMINIMAL_STACK_SIZE, (void *) LED4, 3, NULL );
	xTaskCreate(vTaskFunction_LEDToggle, "LED6 Task", configMINIMAL_STACK_SIZE, (void *) LED6, 4, NULL );

    // Start the task scheduler
	vTaskStartScheduler();

	return 0;
}

//----------------Function Definitions-------------------//
/*-----------------------------------------------------
* Function: vTaskFunction_LEDToggle
*
* Description: This task function toggles a specified
*              LED at a specified frequency defined in 
*              the array pointed to by 'pvLED_blink_specs'
*
* param a: void *pvLED_blink_specs: pointer to an array
*          of uint16_t values, the first of which is 
*          a number corresponding to pin in PORTB, where
*          an LED is connect, the second is a value 
*          corresponding to a delay in milliseconds to 
*          be used to block the task to run at a given 
*          frequency.
*
* return: void
*--------------------------------------------------------*/
void vTaskFunction_LEDToggle(void *pvLED_blink_specs)
{
	// variable to hold ticks value of last task run
	portTickType xLastWakeTime;
	// pointer to the value that describes which LED to toggle
	uint16_t *LED_num = (unsigned char *) pvLED_blink_specs; 
	// delay value in ms corresponding to a given toggle frequency
	uint16_t LED_delay_ms = *(LED_num +1);
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	for (;;)  
	{
		// toggle the LED pin
		PORTB ^= (1<<*LED_num);
		// periodically run every 'LED_delay_ms' milliseconds
		vTaskDelayUntil(&xLastWakeTime, LED_delay_ms/portTICK_RATE_MS);
	}
}