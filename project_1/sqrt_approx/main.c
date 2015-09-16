/*----------------------------------------------------------------------------
LAB EXERCISE 5.2 - SQUARE ROOT APPROXIMATION
 ----------------------------------------
Write an assembly code subroutine to approximate the square root of an 
argument using the bisection method. All math is done with integers, so the 
resulting square root will also be an integer

GOOD LUCK!
 *----------------------------------------------------------------------------*/

__asm int my_sqrt(int x){
	
	MOVS r1, #0x00 ; value of a
	MOVS r2, #0xFF ; value of b
	MOVS r3, #0x01 ; value of c
	MOVS r5, #0x00 ; initialize old c
loop
	CMP r5, r3   
	BEQ exit	; Exit if old c = c
	MOVS r5, r3 ; save old c in r4
	ADDS r3, r1, r2 ; c = a+b
	ASRS r3, r3, #1
	MOVS r4, r3;
	MULS r4, r3, r4 ; c*c
	CMP r4, r0
	BEQ exit
	BGT setb
	MOVS r1, r3 ; c -> a
	B loop
setb
	MOVS r2, r3 ; c -> b
	B loop
exit
	MOVS r0, r3 ; return value
	BX lr
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main(void){
	volatile int r, j=0;
	int i;
  r = my_sqrt(0);     // should be 0
  r = my_sqrt(25);    // should be 5
	r = my_sqrt(133); 	// should be 11
  for (i=0; i<10000; i++){
		r = my_sqrt(i);
    j+=r;
  }
	while(1)
		;
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************
