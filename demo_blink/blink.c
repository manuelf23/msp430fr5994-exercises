/*
 * blink.c
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */

#include <blink.h>
#include <msp430.h>
#define LONG_STACK_BLINK        50
void blink_init(blink_control_t *bcp)
{

    P1DIR |= 0x01;                          // Set P1.0 to output direction
    P1OUT &= ~0x01;
}
void blink_process(blink_control_t *bcp)
{
    extern Tcb tcb_ppal;
    while(1)
    {
        P1OUT ^= 0x01;
        Ejecute_cr (&tcb_ppal);
    }
}






