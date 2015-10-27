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
typedef struct lowPowerAdcBoundaries
{
    int32_t upperBoundary;
    int32_t lowerBoundary;
} lowPowerAdcBoundaries_t;
volatile static uint16_t adcValue = 0;      /*! ADC value */
static uint32_t adcrTemp25 = 0;             /*! Calibrated ADCR_TEMP25 */
static uint32_t adcr100m = 0;
volatile bool conversionCompleted = false;  /*! Conversion is completed Flag */
//adc16_flag_t kAdcChnConvCompleteFlag;
///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////

/*!
 * Parameters calibration: VDD and ADCR_TEMP25
 */
void calibrateParams(void)
{
    adc16_hw_average_config_t userHwAverageConfig;
#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    adc16_calibration_param_t adcCalibrationParam;
#endif
    adc16_converter_config_t adcUserConfig;
    adc16_chn_config_t adcChnConfig;
//    pmc_bandgap_buffer_config_t pmcBandgapConfig = {
//        .enable = true,
//#if FSL_FEATURE_PMC_HAS_BGEN
//        .enableInLowPower = false,
//#endif
//#if FSL_FEATURE_PMC_HAS_BGBDS
//        .drive = kPmcBandgapBufferDriveLow,
//#endif
//    };
    uint32_t bandgapValue = 0;  /*! ADC value of BANDGAP */
    uint32_t vdd = 0;           /*! VDD in mV */

    // Initialization ADC for
    // 16bit resolution, interrupt mode disabled, hw trigger disabled.
    // normal convert speed, VREFH/L as reference,
    // enaable continuous convert mode.
//    ADC16_DRV_StructInitUserConfigDefault(&adcUserConfig);
//    adcUserConfig.resolution = kAdc16ResolutionBitOf16;
//    adcUserConfig.continuousConvEnable = true;
//    adcUserConfig.clkSrc = kAdc16ClkSrcOfAsynClk;
    ADC16_DRV_Init(ADC_0, &adcUserConfig);

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    // Auto calibration
    ADC16_DRV_GetAutoCalibrationParam(ADC_0, &adcCalibrationParam);
    ADC16_DRV_SetCalibrationParam(ADC_0, &adcCalibrationParam);
#endif

    // Enable BANDGAP reference voltage
//    PMC_HAL_BandgapBufferConfig(PMC_BASE_PTR, &pmcBandgapConfig);


//#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
//    // Use hardware average to increase stability of the measurement.
//    userHwAverageConfig.hwAverageEnable = true;
//    userHwAverageConfig.hwAverageCountMode = kAdc16HwAverageCountOf32;
//    ADC16_DRV_ConfigHwAverage(ADC_0, &userHwAverageConfig);
//#endif // FSL_FEATURE_ADC16_HAS_HW_AVERAGE

//    adcChnConfig.chnIdx                  = kAdcChannelBandgap;
//    adcChnConfig.diffConvEnable          = false;
//    adcChnConfig.convCompletedIntEnable  = false;
    ADC16_DRV_ConfigConvChn(ADC_0, CHANNEL_0, &adcChnConfig);
    ADC16_DRV_ConfigConvChn(ADC_0, CHANNEL_1, &adcChnConfig);
    ADC16_DRV_ConfigConvChn(ADC_0, CHANNEL_2, &adcChnConfig);

    // Wait for the conversion to be done
//    ADC16_DRV_WaitConvDone(ADC_0, CHANNEL_0);
    // Get current ADC BANDGAP value
//    bandgapValue = ADC16_DRV_GetConvValueSigned(ADC_0, CHANNEL_0);
    // ADC stop conversion
//    ADC16_DRV_PauseConv(ADC_0, CHANNEL_0);
    // Get VDD value measured in mV: VDD = (ADCR_VDD x V_BG) / ADCR_BG
//    vdd = ADCR_VDD * V_BG / bandgapValue;
    // Calibrate ADCR_TEMP25: ADCR_TEMP25 = ADCR_VDD x V_TEMP25 / VDD
//    adcrTemp25 = ADCR_VDD * V_TEMP25 / vdd;
    // ADCR_100M = ADCR_VDD x M x 100 / VDD
//    adcr100m = (ADCR_VDD * M) / (vdd * 10);

    // Disable BANDGAP reference voltage
//    pmcBandgapConfig.enable = false;
//    PMC_HAL_BandgapBufferConfig(PMC_BASE_PTR, &pmcBandgapConfig);
}   // end calibrate parameters


/* Calculate the current temperature */
int32_t GetCurrentTempValue(void)
{
    int32_t currentTemperature = 0;

    // Temperature = 25 - (ADCR_T - ADCR_TEMP25) * 100 / ADCR_100M
    currentTemperature = (int32_t)(STANDARD_TEMP - ((int32_t)adcValue - (int32_t)adcrTemp25) * 100 / (int32_t)adcr100m);

    return currentTemperature;
}  // end GetCurrentTempValue

/* Calculate the average temperature and set boundaries */
lowPowerAdcBoundaries_t TempSensorCalibration(uint32_t updateBoundariesCounter,
                                                     int32_t *tempArray)
{
    uint32_t avgTemp = 0;
    lowPowerAdcBoundaries_t boundaries;

    for(int i = 0; i < updateBoundariesCounter; i++)
    {
        avgTemp += tempArray[i];
    }
    // Get average temperature
    avgTemp /= updateBoundariesCounter;

    // Set upper boundary
    boundaries.upperBoundary = avgTemp + UPPER_VALUE_LIMIT;

    // Set lower boundary
    boundaries.lowerBoundary = avgTemp - LOWER_VALUE_LIMIT;

    return boundaries;
}  // End TempSensor Calibration



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
    lowPowerAdcBoundaries_t boundaries;
    int32_t currentTemperature = 0;
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

   // Calibrate param Temperature sensor
   calibrateParams();

   // Initialize ADC
//   if (init_adc(ADC_0))
//   {
//       PRINTF("Failed to do the ADC init\n");
//       return -1;
//   }

   // Warm up microcontroller and allow to set first boundaries
    while(updateBoundariesCounter < (UPDATE_BOUNDARIES_TIME * 2))
    {
        while(!conversionCompleted);
        currentTemperature = GetCurrentTempValue();
        tempArray[updateBoundariesCounter] = currentTemperature;
        updateBoundariesCounter++;
        conversionCompleted = false;
    }

    // Temp Sensor Calibration
    boundaries = TempSensorCalibration(updateBoundariesCounter, tempArray);
    updateBoundariesCounter = 0;



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
    	// wait for it
    	while (   !ADC16_DRV_GetChnFlag(ADC_0, CHANNEL_2, kAdcChnConvCompleteFlag) )
        	{  ;
        	}
    	frequency = ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_2);


    	while (  !ADC16_DRV_GetChnFlag(ADC_0, CHANNEL_1, kAdcChnConvCompleteFlag) )
    	{  ;
    	}
    	// Get current ADC value
    	adcValue = ADC16_DRV_GetConvValueRAW(ADC_0, CHANNEL_1);
    	// Set conversionCompleted flag. This prevents an wrong conversion in main function
    	conversionCompleted = true;

    	// Get temperature if conversion complete
    	if (conversionCompleted)
    	{
    		// Get current Temperature Value
    		currentTemperature = GetCurrentTempValue();
    		// Store temperature values that are going to be use to calculate average temperature
    		tempArray[updateBoundariesCounter] = currentTemperature;
    		// Call update function
    		if(updateBoundariesCounter >= (UPDATE_BOUNDARIES_TIME))
    		{
    			boundaries = TempSensorCalibration(updateBoundariesCounter, tempArray);
    			updateBoundariesCounter = 0;
    		}
    		else
    		{
    			updateBoundariesCounter++;
    		}
    		// Clear conversionCompleted flag
    		conversionCompleted = false;
    	}
    }

//  ECEN 5003 add code as indicated

//  calculate flow()

//  4-20 output ()    // use TMP0 channel 3  proporional rate to flow

//  Pulse output()   // use TMP0 channel 4  propotional rate to frequency

//  LCD_Display()   // use the SPI port to send flow number

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
