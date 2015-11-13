/*----------------------------------------------------------------------------
 Utilities header file
 *----------------------------------------------------------------------------*/
#ifndef UTILS_H
#define UTILS_H

#include <stm32f4xx.h>

//RCC Register Masks
#define RCC_CLOCK			(*((unsigned int *)0x40023830u))
#define RCC_PORTA_MASK		(0x01u)
#define RCC_PORTB_MASK		(0x02u)
#define RCC_PORTC_MASK		(0x04u)
#define RCC_PORTD_MASK		(0x08u)
#define RCC_PORTH_MASK		(0x80u)

//GPIO registers
#define MODER(x)			(0x03 << x*2)
#define MODER_0(x)			(0x01 << x*2)
#define MODER_1(x)			(0x02 << x*2)
#define OTYPER(x)			(0x01 << x)
#define OSPEEDR(x)			(0x03 << x*2)
#define OSPEEDR_0(x)		(0x01 << x*2)
#define OSPEEDR_1(x)		(0x02 << x*2)
#define PUPDR(x)			(0x03 << x*2)
#define PUPDR_0(x)			(0x01 << x*2)
#define PUPDR_1(x)			(0x02 << x*2)
#define IDR(x)				(0x01 << x)
#define ODR(x)				(0x01 << x)
#define BSRR_L(x)			(0x01 << x)
#define BSRR_H(x)			(0x10000 << x)
#define LCKR(x)				(0x01 << x)

/* Extracts pin number from the pindef.h file */
/* Please do not change */
#define GET_PIN_INDEX(pin) ((pin) & 0x0F)

#endif
// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
