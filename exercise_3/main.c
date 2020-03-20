#include <msp430.h> 
#include <timer.h>
#include <display.h>

/**
 * main.c
 */
timer_control_t tcp;
//datos_control_t dcp;
//rx_control_t rcp;
dy_control_t dycp;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers


    Timer_init(&tcp);
    display_init(&dycp, &tcp);
    //rx_init(&rcp, &tcp);
    //datos_init(&dcp, &tcp, &rcp, &dycp);

	while(1)
	{
	    Timer_Process(&tcp);
	    display_process(&dycp);
	    display_mostrar(&dycp, 188);
	    display_camb_blink(&dycp, BLINK_2HZ);
	}
}
