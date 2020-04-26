#include <msp430.h> 
#include <timer.h>
#include <display.h>
#include <datos.h>
#include <rx.h>
#include <xmodem.h>
#include <teclado.h>

/**
 * main.c
 */
timer_control_t tcp;
datos_control_t dcp;
rx_control_t rcp;
dy_control_t dycp;
xmodem_control_t xmodem;
teclado_control_t tclp;

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
    rx_init(&rcp);
    datos_init(&dcp, &tcp, &dycp);
    xmodem_init(&xmodem, &tcp, &rcp , &dcp);
    teclado_init(&tclp, &tcp, &dcp);

    __bis_SR_register(GIE);     // Enter LPM0 w/ interrupt
	while(1)
	{
	    //Timer_Process(&tcp);
	    rx_process(&rcp);
        xmodem_process(&xmodem);
        datos_process(&dcp);
        display_process(&dycp);
        teclado_process(&tclp);
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    char n;
    //TA0CTL &= ~TAIFG; // -bajo la bandera del timer
    // Revisa la tabla de per�odos y procesa los que est�n activos
    //for (n = tcp.tp_c ; n; n--)
    __disable_interrupt();
    for (n=0; n < tcp.tp_c ; n++)
    if (tcp.tp[n].banderas & TIMER_ACTIVO)
    {
        tcp.tp[n].contador --;
        if ( (tcp.tp[n].contador)==0 )
        {
            tcp.tp[n].banderas |= TIMER_FINALIZADO;
            tcp.tp[n].contador = tcp.tp[n].valor;
        };

    };

    // Revisa la tabla de timeouts y procesa los que est�n activos
    //for (n = tcp.to_c; n; --n)
    for (n=0; n < tcp.to_c ; n++)
    if (tcp.to[n].banderas & TIMER_ACTIVO)
    {
        tcp.to[n].contador --;
        if ( tcp.to[n].contador ==0 )
        {
            tcp.to[n].banderas |= TIMER_FINALIZADO;
            tcp.to[n].banderas &= ~TIMER_ACTIVO ;
        };
    };
    __enable_interrupt();
    //TA0CCR0 += 50000;
}
