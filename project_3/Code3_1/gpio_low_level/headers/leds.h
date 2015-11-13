/*----------------------------------------------------------------------------
 LED header file
 *----------------------------------------------------------------------------*/
#ifndef	LEDS_H
#define	LEDS_H

#include <PinNames.h>
#include "pindef.h"
#include "utils.h"

//Get pin numbers from the pindef.h file
#define RED_LED GET_PIN_INDEX(Dout0)
#define GREEN_LED GET_PIN_INDEX(Dout1)
#define BLUE_LED GET_PIN_INDEX(Dout2)

//Function prototypes
void init_RGB(void);
void toggle_all(void);
void toggle_r(void);
void toggle_g(void);
void toggle_b(void);

#endif
// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
