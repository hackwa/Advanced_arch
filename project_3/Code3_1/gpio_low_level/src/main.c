/*----------------------------------------------------------------------------
LAB EXERCISE 6 - LOW LEVEL DIGITAL IO
 ----------------------------------------
Digital input: switches on a breadboard
Digital output: RGB LED on the board
Function: Use the GPIO peripheral to control LEDs

GOOD LUCK!
 *----------------------------------------------------------------------------*/
#include "leds.h"
#include "switches.h"

//Simple software debouncing
void delay(float time){
	volatile int i;
	for(i=0; i<1000000*time; i++);
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
	//Initialise switches and LEDs
	init_RGB();
	init_switches();
	
	//Infinite loop
	while(1){
		//Check which button was pressed and toggle a corresponding LED
		if(!(GPIOA->IDR >> SWITCH_1 & 0x1)){
			toggle_r();
			delay(1);
		}
		
		//Write your code here
		
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
