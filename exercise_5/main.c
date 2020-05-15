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
	    //rx_process(&rcp);
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
    //__disable_interrupt();
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
    //__enable_interrupt();
    //TA0CCR0 += 50000;
}


#pragma vector=EUSCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
    switch(__even_in_range(UCA3IV, USCI_UART_UCTXCPTIFG))
        {
            case USCI_NONE: break;
            case USCI_UART_UCRXIFG:
                while(!(UCA3IFG&UCTXIFG));
                {


                    rcp.rx_buffer[rcp.rx_llenado] = UCA3RXBUF;
                    rcp.rx_uso ++;
                    rcp.rx_llenado ++;
                    if(rcp.rx_llenado == DATOS_RX_BUFFER_TAMANO)
                    {
                        rcp.rx_llenado = 0;
                    }
                    if(rcp.rx_uso == DATOS_RX_BUFFER_TAMANO) // se guardó el dato y ahora se pregunta si se lleno la cola
                    {
                        rcp.rx_full = 1;
                        UCA3IE &= UCRXIE_0; // deshabilitar interrupciones de la UART (vuelven a habilatrse en la iterfaz que saca datos de la cola cuando haya espacio)
                    }
                }
                //UCA3TXBUF = UCA3RXBUF
                //__no_operation();
                break;
            case USCI_UART_UCTXIFG: break;
            case USCI_UART_UCSTTIFG: break;
            case USCI_UART_UCTXCPTIFG: break;
            default: break;
        }
}
