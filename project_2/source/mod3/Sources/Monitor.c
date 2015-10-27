/*-----------------------------------------------------------------------------
--                                                                           --
--              ECEN 5003 Mastering Embedded System Architecture             --
--                  Project 2 Module 2                                       --
--                Microcontroller Firmware                                   --
--                      Monitor.c                                             --
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
--      Copyright (c) 2015 Tim Scherr All rights reserved.
--
*/              

#include <stdio.h>
#include "shared.h"
#include "fsl_gpio_hal.h"


/*******************************************************************************
* Set Display Mode Function
* Function determines the correct display mode.  The 3 display modes operate as 
*   follows:
*
*  NORMAL MODE       Outputs only mode and state information changes   
*                     and calculated outputs
*
*  QUIET MODE        No Outputs
*
*  DEBUG MODE        Outputs mode and state information, error counts,
*                    register displays, sensor states, and calculated output
*
*
* There is deliberate delay in switching between modes to allow the RS-232 cable 
* to be plugged into the header without causing problems. 
*******************************************************************************/

void set_display_mode(void)
{
  UART_direct_msg_put("\r\nSelect Mode");
  UART_direct_msg_put("\r\n Hit NOR - Normal");
  UART_direct_msg_put("\r\n Hit QUI - Quiet");
  UART_direct_msg_put("\r\n Hit DEB - Debug" );
  UART_direct_msg_put("\r\n Hit V - Version#");
  UART_direct_msg_put("\r\n Hit REG - Registers Status");
  UART_direct_msg_put("\r\n HIT MEM - Memory Display\r\n");
  UART_direct_msg_put("\r\nSelect:  ");
  
}


//******************************************************************************
// UART Input Message
//******************************************************************************
void chk_UART_msg(void)
{
   UCHAR j;
   while( UART_input() )      // becomes true only when a byte has been received
   {                                    // skip if no characters pending
      j = UART_get();                 // get next character

      if( j == '\r' )          // on a enter (return) key press
      {                // complete message (all messages end in carriage return)
         UART_msg_put("->");
         UART_msg_process();
      }
      else 
      {
         if ((j != 0x02) )         // if not ^B
         {                             // if not command, then   
            UART_put(j);              // echo the character   
         }
         else
         {
           ;
         }
         if( j == '\b' ) 
         {                             // backspace editor
            if( msg_buf_idx != 0) 
            {                       // if not 1st character then destructive 
               UART_msg_put(" \b");// backspace
               msg_buf_idx--;
            }
         }
         else if( msg_buf_idx >= MSG_BUF_SIZE )  
         {                                // check message length too large
            UART_msg_put("\r\nToo Long!");
            msg_buf_idx = 0;
         }
         else if ((display_mode == QUIET) && (msg_buf[0] != 0x02) && 
                  (msg_buf[0] != 'D') && (msg_buf[0] != 'N') && 
                  (msg_buf[0] != 'V') &&
                  (msg_buf_idx != 0))
         {                          // if first character is bad in Quiet mode
            msg_buf_idx = 0;        // then start over
         }
         else {                        // not complete message, store character
 
            msg_buf[msg_buf_idx] = j;
            msg_buf_idx++;
            if (msg_buf_idx > 2)
            {
               UART_msg_process();
            }
         }
      }
   }
}

//********************************************************************************************************
// UART Input Message Processing
//********************************************************************************************************
void UART_msg_process(void)
{
   UCHAR chr,err=0;
   unsigned char  data;


   if( (chr = msg_buf[0]) <= 0x60 ) 
   {      // Upper Case
      switch( chr ) 
      {
         case 'D':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'B') && (msg_buf_idx == 3)) 
            {
               display_mode = DEBUG;
               UART_direct_msg_put("\r\nMode=DEBUG\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'M':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'M') && (msg_buf_idx == 3))
            {
               display_mode = MEMORY;
               UART_direct_msg_put("\r\nMode=MEMORY\n");
               //display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'N':
            if((msg_buf[1] == 'O') && (msg_buf[2] == 'R') && (msg_buf_idx == 3)) 
            {
               display_mode = NORMAL;
               UART_direct_msg_put("\r\nMode=NORMAL\n");
               //display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'Q':
            if((msg_buf[1] == 'U') && (msg_buf[2] == 'I') && (msg_buf_idx == 3)) 
            {
               display_mode = QUIET;
               UART_direct_msg_put("\r\nMode=QUIET\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'R':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'G') && (msg_buf_idx == 3))
            {
               display_mode = REGISTER;
               UART_direct_msg_put("\r\nMode=REGISTER\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'V':
            display_mode = VERSION;
            UART_direct_msg_put( CODE_VERSION );
            UART_direct_msg_put("\r\nSelect  ");
            display_timer = 0;
            break;
                
         default:
            err = 1;
      }
   }

   else 
   {                                 // Lower Case
      switch( chr ) 
      {
        default:
         err = 1;
      }
   }

   if( err == 1 )
   {
      UART_msg_put("\n\rError!");
   }   
   else if( err == 2 )
   {
      UART_msg_put("\n\rNot in DEBUG Mode!");
   }   
   else
   {
    msg_buf_idx = 0;          // put index to start of buffer for next message
      ;
   }
   msg_buf_idx = 0;          // put index to start of buffer for next message


}


//*****************************************************************************
// IS_HEX Function
// Function takes a single ASCII character and returns 
//   1 if hex digit, 0 otherwise.
//*****************************************************************************
UCHAR is_hex(UCHAR c)
{
   if( (((c |= 0x20) >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'))  )
      return 1;
   return 0;
}

/*******************************************************************************
* DEBUG and DIAGNOSTIC Mode UART Operation
*******************************************************************************/
void monitor(void)
{

/**********************************/
/*     Spew outputs               */
/**********************************/

   switch(display_mode)
   {
      case(QUIET):
         {
             UART_msg_put("\r\n ");
             display_flag = 0;
         }  
         break;
      case(VERSION):
         {
             display_flag = 0;
         }  
         break;         
      case(NORMAL):
         {
            if (display_flag == 1)
            {
               UART_msg_put("\r\nNORMAL ");
               UART_msg_put(" Flow: ");
               UART_hex_put(0x1230);
               // ECEN 5003 add code as indicated
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Temp: ");
               UART_hex_put(0x3456);
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Freq: ");
               UART_hex_put(0x7890);
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               display_flag = 0;
               //GPIO_HAL_TogglePinOutput(PTD,1);
            }
         }  
         break;
      case(MEMORY):
         {
          if (display_flag == 1)
          {
             UART_msg_put("\r\nMEMORY ");
             UART_msg_put(" Memory Dump:  \r\n");
             display_flag = 0;
           }
         }
         break;
      case(REGISTER):
         {
          if (display_flag == 1)
          {
             UART_msg_put("\r\n REGISTER ");
             UART_msg_put("Register Values: \r\n");
             UART_msg_put("R1: \tR2: \tR3: \tR4: \tR5: \r\n");
             UART_msg_put("R6: \tR7: \tR8: \tR9: \tR10: \r\n");
             UART_msg_put("R11: \tR12: \tR13: \tR14: \tR15: \r\n");
             display_flag = 0;
           }
         }
         break;
      case(DEBUG):
         {
            if (display_flag == 1)
            {
               UART_msg_put("\r\nDEBUG ");
               UART_msg_put(" Flow: ");
               // ECEN 5003 add code as indicated               
               //  add flow data output here, use UART_hex_put or similar for
               UART_hex_put(0x1230);
               // numbers
               UART_msg_put(" Temp: ");
               //  add flow data output here, use UART_hex_put or similar for 
               UART_hex_put(0x3456);
               // numbers
               UART_msg_put(" Freq: ");
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_hex_put(0x7890);
               UART_msg_put("\r\nError Counts: ");
               //SUART_direct_msg_put("Error: ");
               UART_hex_put(0X12);
               UART_msg_put(" Sensor States: Active \r\n");

               //  Create a display of  error counts, sensor states, and
               //  ARM Registers R0-R15
               //  Create a command to read a section of Memory and display it
               
               
               //  Create a command to read 16 words from the current stack 
               // and display it in reverse chronological order.
              
              
               // clear flag to ISR      
               display_flag = 0;
             }   
         }  
         break;

      default:
      {
         UART_msg_put("Mode Error");
      }  
   }
}  
