/*
 * toggle.c
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */


#include <toggle.h>
#include <msp430.h>
#include <stdint.h>

#define LONG_STACK_BLINK        50
void toggle_init(toggle_control_t *tgcp)
{
    tgcp->val = 50000;
}

void toggle_process(toggle_control_t *tgcp)
{
    extern Tcb tcb_ppal;
    for(;;) {
            volatile unsigned int i;            // volatile to prevent optimization
            Ejecute_cr (&tcb_ppal);
            i = tgcp->val;                          // SW Delay
            do i--;
            while(i != 0);
        }
}
