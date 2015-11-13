/*----------------------------------------------------------------------------
 Switches header file
 *----------------------------------------------------------------------------*/
#ifndef SWITCHES_H
#define SWITCHES_H

#include <PinNames.h>
#include "pindef.h"
#include "utils.h"

//Get pin numbers from the pindef.h file
#define SWITCH_1 GET_PIN_INDEX(Din0)
#define SWITCH_2 GET_PIN_INDEX(Din1)
#define SWITCH_3 GET_PIN_INDEX(Din2)
#define SWITCH_4 GET_PIN_INDEX(Din3)

//Function prototypes
void init_switches(void);

#endif
// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
