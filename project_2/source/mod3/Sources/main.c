/* ###################################################################
**     Filename    : main.c
**     Project     : Project2DC
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-06, 19:02, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "DbgCs1.h"
#include "adConv1.h"
#include "fsl_spi_hal.h"
#include "fsl_tpm_hal.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif

/* User includes (#include below this line is not maintained by Processor Expert) */
extern volatile uint16_t SwTimerIsrCounter = 0U;
#define MAIN
#include "shared.h"
#include "MKL25Z4.h"
#include "fsl_gpio_hal.h"
#undef MAIN
#include <stdio.h>
#define MAIN
///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define ADC_0                   (0U)
#define CHANNEL_0               (0U)
#define CHANNEL_1               (1U)
#define CHANNEL_2               (2U)
#define LED_ON                  (0U)
#define LED_OFF                 (1U)
/*!
 * @brief These values are used to get the temperature. DO NOT MODIFY
 * The method used in this demo to calculate temperature of chip is mapped to
 * Temperature Sensor for the HCS08 Microcontroller Family document (Document Number: AN3031)
 */
#define ADCR_VDD                (65535U)    /*! Maximum value when use 16b resolution */
#define V_BG                    (1000U)     /*! BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25                (716U)      /*! Typical VTEMP25 in mV */
#define M                       (1620U)     /*! Typical slope: (mV x 1000)/oC */
#define STANDARD_TEMP           (25)

#define UPPER_VALUE_LIMIT       (1U)        /*! This value/10 is going to be added to current Temp to set the upper boundary*/
#define LOWER_VALUE_LIMIT       (1U)        /*! This Value/10 is going to be subtracted from current Temp to set the lower boundary*/
#define UPDATE_BOUNDARIES_TIME  (20U)       /*! This value indicates the number of cycles needed to update boundaries. To know the Time it will take, multiply this value times LPTMR_COMPARE_VALUE*/
//#define kAdcChannelTemperature  (kAdc16Chn26)       /*! ADC channel of temperature sensor */
//#define kAdcChannelBandgap      (kAdc16Chn27)       /*! ADC channel of BANDGAP */

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
struct SPI_type *lcd;
struct TPM_type *flowTPM;
struct TPM_type *frequencyTPM;
tpm_pwm_param_t *pwmControl;


long getFrequency(){
	 char *buf[1000];
	    int i = 0;
	    int c = 0;
	    long binary_data[1000];
	    FILE * data = fopen("ECEN5003Data7.txt","r");
	    for(i;i<1000;i++)
	    {
	        buf[i] = (char*) calloc(5,sizeof(char));
	        fgets(buf[i],sizeof(buf[i]),data);
	        binary_data[i] = strtol(buf[i],0,16);
	//        printf("%d\n",binary_data[i] );
	    }
	    long max = binary_data[0];
	    for ( i=0; i<1000; i++ )
	    {
	    	if (binary_data[i] > max)
	    	{max = binary_data[i];}
	    }
	    int array[1000],x=0;
	    for (c;c<1000;c++)
	    {
	    	if (binary_data[c]==max)
	    	{
	    		array[x] = c;x++;
	 //           printf("Xrray: %d\n", c);
	    	}
	    }
	    int t = 2*(array[1]-array[0]);
	    long f = pow(10,4);
	    f = f/t;
//	    UART_direct_msg("Frequency %lu\n",f );
	    return f;
}

float getFlow(long f, int32_t temp){
    float d,v0,v,pid,a,tol = 10,b,density,viscosity, area;
    d = 0.0127;
    pid =0.07366;
    v0=1;
    float n = temp-3.9863;
    float area1 = 3.142*pow((double)d,2);
    area = area1/4;
    a = 508929.2*(temp+68.12963);
    b = 247.8/(296.15-140);
    float y = (temp+288.9414)/a;
    density = 1000*(1-(y*pow(n,2)));
    viscosity = 2.4*pow(10,-5)*10*b;
    while(tol>0.01)
    {
    	float re = (density*v0*pid)/viscosity;
    	float pth = 1.036/sqrt(re);
    	float st = 0.2684 - pth;
    	v = f*d/st;
    	tol = (abs(v0-v)/v0)*100;
    	v0 = v;
    }
  //  UART_msg_put("Velocity: %f\n",v0 );
    float vFeetPerSec, flow;
    vFeetPerSec = v0 * 3.28;
    pid = 2.9;
    flow = 2.45 * pid * pid * vFeetPerSec;
    return flow;
}
///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data
    // uint8_t receiveBuff;
    uint32_t  count = 0;   
    uint16_t frequency = 0;
    int32_t currentTemperature = 23;
    int32_t conversionCompleted = 0;
    uint32_t updateBoundariesCounter = 0;
    int32_t tempArray[UPDATE_BOUNDARIES_TIME * 2];
    long freq;
    float flowrate;
    uint32_t frequencyTPMChannel = 4, freqmax = 2000;
    uint32_t flowTPMChannel = 3, dutyCycle;
    uint32_t flowmax = 1000;

// initialize serial buffer pointers
   rx_in_ptr =  rx_buf; /* pointer to the receive in data */
   rx_out_ptr = rx_buf; /* pointer to the receive out data*/
   tx_in_ptr =  tx_buf; /* pointer to the transmit in data*/
   tx_out_ptr = tx_buf; /*pointer to the transmit out */
    
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  /* ECEN 5003 User code here */
  // Print the initial banner
    PRINTF("\r\nHello World!\n\n\r");

   /* send a message to the terminal  */ 
   UART_direct_msg_put("\r\nSystem Reset\r\nCode ver. ");
   UART_direct_msg_put( CODE_VERSION );
   UART_direct_msg_put("\r\n");
   UART_direct_msg_put( COPYRIGHT );
   UART_direct_msg_put("\r\n");

   set_display_mode(); 
   
   GPIO_HAL_SetPortDir(PTB,1);

   SPI_HAL_Init(lcd);
   SPI_HAL_Enable(lcd);
 //  _spi_master_slave_mode self;
   spi_master_slave_mode_t mode;
   SPI_HAL_SetMasterSlave(lcd, mode);
   uint32_t bitsPerSec = 9600;
   uint32_t sourceClockHz = 8000000;
   SPI_HAL_SetBaud(lcd,bitsPerSec,sourceClockHz);

    while(1)
    {
        // Main routine 

    count++;                  // counts the number of times through the loop
//    __enable_interrupt();
//    __clear_watchdog_timer();

    serial();            // Polls the serial port
    chk_UART_msg();     // checks for a serial port message received
    monitor();           // Sends serial port output messages depending
                         //  on commands received and display mode

    //   Get ADC Conversion every 100 uS
    if (adc_flag)
    {
    	conversionCompleted = 1;
    	// Get temperature if conversion complete
    	if (conversionCompleted)
    	{
    		// Get current Temperature Value from ADC
    		currentTemperature = 23;
    		// Clear conversionCompleted flag
    		conversionCompleted = 0;
    	}
    }

//  ECEN 5003 add code as indicated
freq = getFrequency();
//  calculate flow()
flowrate = getFlow(freq,currentTemperature);
pwmControl->uFrequencyHZ = 8000000;
//  4-20 output ()    // use TPM0 channel 3  proporional rate to flow
dutyCycle = (flowmax / flowrate) * 100;
pwmControl->uDutyCyclePercent = dutyCycle;
TPM_HAL_EnablePwmMode(frequencyTPM,pwmControl,flowTPMChannel);
//  Pulse output()   // use TPM0 channel 4  propotional rate to frequency
dutyCycle = (freqmax / freq) * 100;
pwmControl->uDutyCyclePercent = dutyCycle;
TPM_HAL_EnablePwmMode(flowTPM,pwmControl,frequencyTPMChannel);
//TPM_HAL_EnablePwmMode
//  LCD_Display()   // use the SPI port to send flow number
    uint32_t data;
    SPI_HAL_WriteData(lcd,data);

//  End ECEN 5003 code addition


        if ((SwTimerIsrCounter & 0x1FFF) > 0x0FFF)
        {
        	GPIO_HAL_TogglePinOutput(PTB,18);  // Toggle Green LED
        }
    }


  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
