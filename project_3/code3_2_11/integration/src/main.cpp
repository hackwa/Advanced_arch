/*----------------------------------------------------------------------------
LAB EXERCISE 11.4- SPI and I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
 Display the temperature from the virtual DS1631 temperature sensor on the 
      virtual LCD
 
 Input: virtual DS1631 temperature sensor
 Output: virtual LCD display
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "NHD_0216HZ.h"
#include "DS1631.h"
#include "pindef.h"


//Define the LCD and the temperature sensor
NHD_0216HZ lcd(SPI_CS,SPI_MOSI,SPI_SCLK);
DS1631 sensor(PB_9, PB_8,0X90);
//Write your code here


//Define a variable to store temperature measurement
float temp;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
	//Initialise the LCD
	lcd.init_lcd();
	//Write your code here
	
	
	while(1){
        char buf[50];
        lcd.clr_lcd();
        temp = sensor.read();
        sprintf(buf,"%f",temp);
        lcd.printf(buf);
        wait(1);
		/*
		Read the temperature from the DS1631
		Update the LCD with new temperature measurement
		*/
		
		//Write your code here
		
		
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
