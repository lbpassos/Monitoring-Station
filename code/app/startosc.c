/*
 * startosc.c
 *
 *  Created on: 17 Mar 2013
 *      Author: lpassos
 * 		edited: -
 */

#include "../mylib/lpc2106.h"

void StartOsc(void) {
    PLLCFG = 0x23;	// Setting M and P values M=4 P=2
    PLLFEED = 0xAA; PLLFEED = 0x55;
    PLLCON = 0x1;		// Enabling the PLL 
    PLLFEED = 0xAA; PLLFEED = 0x55;
    while ( !(PLLSTAT & PLOCK) );   // Wait for the PLL to lock
    PLLCON = 0x3;		// Connect the PLL as the clock source
    PLLFEED = 0xAA; PLLFEED = 0x55;
    VPBDIV = 0x00; 		// Setting pclk to 1/4cclk
}








