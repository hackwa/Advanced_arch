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
#include "fsl_gpio_hal.h"
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

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
    // RX buffers
    //! @param receiveBuff Buffer used to hold received data
    // uint8_t receiveBuff;
	uint32_t  count = 0;
    
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
   GPIO_HAL_SetPinDir(PTB,18,kGpioDigitalOutput);
   GPIO_HAL_SetPinDir(PTD,1,kGpioDigitalOutput);

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
        // First, get character
//        receiveBuff = GETCHAR();

        // Now echo the received character
//        PUTCHAR(receiveBuff);

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
