/*
 * blink.c
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */

#include <blink.h>
#define LONG_STACK_BLINK        50
void blink_init(blink_control_t *bcp, Tcb *tcb_blink, Tcb *tcb_ppal)
{
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    P1OUT &= ~0x01;

    bcp->tcb_blink = tcb_blink;
    bcp->tcb_ppal = tcb_ppal;
    static uint32_t  stack_blink[LONG_STACK_BLINK]= {0};
    Inicie_cr(&bcp->tcb_blink, stack_blink + LONG_STACK_BLINK, blink_process, NULL);
}
void blink_process(blink_control_t *bcp)
{
    while(1)
    {
        P1OUT ^= 0x01;
        Ejecute_cr (&bcp->tcb_ppal);
        //Ejecute_cr (&tcb_toggle);
    }
}
