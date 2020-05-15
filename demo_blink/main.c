//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include <msp430.h>
#include <stdint.h>
#include <corrutinas.h>
#include <blink.h>
#include <toggle.h>


Tcb tcb_blink, tcb_ppal, tcb_toggle;
blink_control_t bcp;
toggle_control_t tgcp;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    /*
    Inicie_cr(&tcb_blink, stack_blink + LONG_STACK_BLINK, blink, NULL);
    Inicie_cr(&tcb_toggle, stack_toggle + LONG_STACK_BLINK, toggle, 50000);
    */
    blink_init(&bcp, tcb_blink, tcb_ppal);
    toggle_init(&tgcp, &tcb_toggle, &tcb_ppal);
    tcb_actual=&tcb_ppal;

    while(1){
        Ejecute_cr (&tcb_blink);
        Ejecute_cr (&tcb_toggle);
    }




}

