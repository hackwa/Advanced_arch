/*-----------------------------------------------------------------------------
--                                                                           --
--              ECEN 5003 Mastering Embedded System Architecture             --
--                  Project 2 Module 2                                       --
--                Microcontroller Firmware                                   --
--                      shared.h                                            --
--                                                                           --
-------------------------------------------------------------------------------
--
--  Designed for:  University of Colorado at Boulder
--               
--                
--  Designed by:  Tim Scherr
--  Revised by:  Student's name 
-- 
-- Version: 1.0
-- Date of current revision:  2015-10-10   
-- Target Microcontroller: Freescale MKL25ZVMT4 
-- Tools used:  Freescale Kinetis Design Studio  3.0.0
--              Freescale Kinetis SDK 1.2.0
--              Freescale Processor Expert 
--              Freescale FRDM-KL25Z Freedom Board
--               
-- 
   Functional Description:  
--
--      Copyright (c) 2015 Tim Scherr  All rights reserved.
--
*/              
                          
// #include   
 


/*****************************************************************************
* #defines available to all modules included here
******************************************************************************/ 
#define OFF 0                       /* used for readability */
#define ON 1                        /* used for readability */
#define NO 0                        /* used for readability */
#define YES 1                       /* used for readability */
#define TEN 10

#define TIMER0 TMR0
#define SEC 10000           /* 10000 timer0 interrupts per second (100 usec.) */
                   
#define T100MS 0.1*SEC
#define T2S    2*SEC
                              
#define LED_FLASH_PERIOD .5   /* in seconds */
 
#define CLOCK_FREQUENCY_MHZ 8
#define CODE_VERSION "1.0 2015/10/10"   /*   YYYY/MM/DD  */
#define COPYRIGHT "Copyright (c) University of Colorado" 
     
 enum boolean { FALSE, TRUE };
 enum dmode {QUIET, NORMAL, DEBUG, VERSION, MEMORY, REGISTER};
 
 typedef unsigned char UCHAR;
 typedef unsigned char bit;

/************************************************************************/
/*             Global Variable definitions                              */
/************************************************************************/
 
 unsigned char Error_status;          // Variable for debugging use
 UCHAR  display_timer;  // 1 second software timer for display   
 UCHAR  display_flag;   // flag between timer interrupt and monitor.c, like
                        // a binary semaphore
 UCHAR  adc_flag;     // flag which times ADC sampling using the timer interrupt semaphore to main
 UCHAR tx_in_progress;                        
 UCHAR *rx_in_ptr; /* pointer to the receive in data */
 UCHAR *rx_out_ptr; /* pointer to the receive out data*/
 UCHAR *tx_in_ptr; /* pointer to the transmit in data*/
 UCHAR *tx_out_ptr; /*pointer to the transmit out */                       
#define RX_BUF_SIZE 10            /* size of receive buffer in bytes */
#define TX_BUF_SIZE 400           /* size of transmit buffer in bytes */
                                                                    
/******************************************************************************
* Some variable definitions are done in the module main.c and are externed in 
* all other modules.  The following section is visible to main.c only.
******************************************************************************/
#ifdef MAIN

 enum dmode display_mode = QUIET;
 
 UCHAR serial_flag = 0;
    
 UCHAR  rx_buf[RX_BUF_SIZE];      /* declare the storage */
 UCHAR  tx_buf[TX_BUF_SIZE];      /* declare the storage */

#define MSG_BUF_SIZE 10
 UCHAR msg_buf[MSG_BUF_SIZE]; // declare the storage for UART received messages
 UCHAR msg_buf_idx = 0;    // index into the received message buffer       

/******************************************************************************
* Some variable definitions are done in the module main.c and are externed in 
* all other modules.  The following section is visible to all modules EXCEPT 
* main.c.
*******************************************************************************/
#else

/**********************/   
/*   Declarations     */
/**********************/

   extern volatile     UCHAR swtimer0;
   extern volatile     UCHAR swtimer1;
   extern volatile     UCHAR swtimer2;
   extern volatile     UCHAR swtimer3; 
   extern volatile     UCHAR swtimer4;    
   extern volatile     UCHAR swtimer5;    
   extern volatile     UCHAR swtimer6;    
   extern volatile     UCHAR swtimer7;    
  
  extern UCHAR serial_flag;
    
  extern enum dmode display_mode;
  
  
  extern UCHAR  rx_buf[];      /* declare the storage */
  extern UCHAR  tx_buf[];      /* declare the storage */

#define MSG_BUF_SIZE 10    
  extern  UCHAR msg_buf[MSG_BUF_SIZE]; // declare the storage for UART received messages
  extern  UCHAR msg_buf_idx;         // index into the received message buffer
#endif

/*************************************************************************************************************
* All function prototypes are externed in all the modules.
*************************************************************************************************************/
extern void monitor(void);  /* located in module monitor.c */
extern void timer0(void);   /* located in module timer0.c */
extern void serial(void);   /* located in module UART.c */

extern void UART_put(UCHAR);                   /* located in module UART.c */
extern UCHAR UART_get(void);                   /* located in module UART.c */
extern UCHAR UART_input(void);                 /* located in module UART.c */
extern void UART_direct_msg_put(UCHAR *);      /* located in module UART.c */
extern void UART_msg_put(UCHAR *);             /* located in module UART.c */
extern void UART_direct_hex_put(UCHAR);        /* located in module UART.c */
extern void UART_direct_put(UCHAR);            /* located in module UART.c */
extern void UART_hex_put(UCHAR);               /* located in module UART.c */
extern void UART_low_nibble_put(UCHAR);        /* located in module UART.c */
extern void UART_high_nibble_put(UCHAR);       /* located in module UART.c */
extern void chk_UART_msg(void);               /* located in module monitor.c */
extern void UART_msg_process(void);          /* located in module monitors.c */
extern void status_report(void);             /* located in module monitor.c */  
extern void set_display_mode(void);          /* located in module monitor.c */


