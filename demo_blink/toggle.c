/*
 * toggle.c
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */


#include <toggle.h>
#include <corrutinas.h>

#define LONG_STACK_BLINK        50
void toggle_init(toggle_control_t *tgcp, Tcb *tcb_toggle, Tcb *tcb_ppal)
{
    tgcp->tcb_toggle = tcb_toggle;
    tgcp->tcb_ppal = tcb_ppal;
    tgcp->val = 10000;
    static uint32_t  stack_toggle[LONG_STACK_BLINK]= {0};
    Inicie_cr(&tcb_toggle, stack_toggle + LONG_STACK_BLINK, toggle_process, tgcp->val);
}

void toggle_process(toggle_control_t *tgcp)
{
    for(;;) {
            volatile unsigned int i;            // volatile to prevent optimization
            Ejecute_cr (&tgcp->tcb_ppal);
            i = tgcp->val;                          // SW Delay
            do i--;
            while(i != 0);
        }
}
