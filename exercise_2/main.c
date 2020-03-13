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
#include <timer.h>
#include <datos.h>
#include <rx.h>
#include <display.h>

timer_control_t tcp;
datos_control_t dcp;
rx_control_t rcp;
dy_control_t dycp;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    Timer_init(&tcp);
    display_init(&dycp, &tcp);
    rx_init(&rcp, &tcp);
    datos_init(&dcp, &tcp, &rcp, &dycp);

    while(1)
    {
        Timer_Process(&tcp);
        datos_process(&dcp);
        display_process(&dycp);
        rx_process(&rcp);
    }
}
