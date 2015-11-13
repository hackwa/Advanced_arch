/*----------------------------------------------------------------------------
LAB EXERCISE 12 - Real-Time Operating System
 ----------------------------------------
	Integrate functions developed in previous modules and run then concurrently
	in the mbed RTOS. The following four threads have to be implemented:
	
	1. Display the temperature on the LCD and to the PC
	2. Adjust the brightness of the RGB LED using a potentiometer
	3. Display an incrementing counter on the LCD and to the PC
	4. Blink an LED
  
  NOTE:  For this lab, the LCD, temp sensor and POT are virtual devices.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "rtos.h"
#include "DS1631.h"
#include "NHD_0216HZ.h"
#include "pindef.h"

/*
Define the mutex
Define the LCD display and the temperature sensor
Define other inputs and outputs
*/

//Write your code here

Mutex lcdaccess;
NHD_0216HZ lcd(SPI_CS,SPI_MOSI,SPI_SCLK);
DS1631 sensor(PB_9, PB_8,0X90);
char temperature[20];
AnalogIn potentiometer(PA_0);
//Take potentiometer reading from serial port

Serial pc(USBTX,USBRX);
PwmOut myled(LED1);
DigitalOut led(LED2);

//Display temperature on the LCD
void temp_thread(void const *args){
    while(1){
        lcdaccess.lock();
        lcd.clr_lcd();
//        lcd.printf(temperature);
        Thread::wait(500);
        lcdaccess.unlock();
    }
}

//Adjust the brightness of the RGB LED
void adjust_brightness(void const *args){
    float brightness;
    while(1){
//        pc.printf("\nReading Potentiometer value..");
        brightness = potentiometer.read();
            myled = brightness;
        Thread::wait(5000);
    }
}
/*
    char arr[4];
    int i;
    float brightness;
	while(1){
        pc.printf("\nEnter Potentiometer Value... :");        	
        i=0;
        while(i < sizeof(arr))
        {
            if(pc.readable())
            {
                arr[i] = pc.getc();
                i++;
            }
        }
        brightness = atof(arr);
        pc.printf("Value Entered is : %f\n",brightness);
*/

//Blink an LED
void led1_thread(void const *args){
	while(1){
        led = !led;
        Thread::wait(500);
 //       pc.printf("here as well");
    }
}

//Display a counter on the LCD
void count_thread(void const *args){
	int counter = 0;
   char buf[4];
    while(1){
        lcdaccess.lock();
        counter+=1;
        lcd.clr_lcd();
        sprintf(buf,"%l",counter);
        pc.printf("\ncounter value: %d",counter);
//        lcd.printf(buf);
        lcdaccess.unlock();
        Thread::wait(5000);
    }
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
	/*
	Initialise and clear the LCD display
	Start all threads
	Wait for timer interrupt    
	*/
    lcd.init_lcd();
    lcd.clr_lcd();
//    Thread printTemp(temp_thread);
    osPriority priority=osPriorityNormal;
    unsigned char *stack_pointer[4]={NULL};
    void *arg=NULL;
    Thread c1(count_thread, arg,
    priority,320 ,stack_pointer[0]);
    Thread c2(led1_thread, arg,
    priority,100,stack_pointer[1]);
    Thread c3(temp_thread, arg,
    priority,150,stack_pointer[2]);
    Thread c4(adjust_brightness, arg,
    priority,300,stack_pointer[3]);
	//write your code here
	while(1){
        Thread::wait(500);
 //       pc.printf("%d",DEFAULT_STACK_SIZE);
        sleep();
    }
}   

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
