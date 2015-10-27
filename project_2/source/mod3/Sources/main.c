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

//  calculate flow()

//  4-20 output ()    // use TMP0 channel 3  proporional rate to flow

//  Pulse output()   // use TMP0 channel 4  propotional rate to frequency

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
