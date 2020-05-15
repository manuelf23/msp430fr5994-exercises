/*
 * timer.c
 *
 *  Created on: Mar 19, 2020
 *      Author: manuelfelipegarciarincon
 */

#include <timer.h>
#include <msp430.h>

void Timer_init(timer_control_t *tcp)
{
    char n;
    //CCTL0 = CCIE;                             // CCR0 interrupt enabled
    //TA0CCTL0 = 0x00;
    TA0CCTL0 |= CCIE;
    TA0CCR0 = 6666; //8.000.000/(1200)
    TA0CTL = TASSEL__SMCLK | MC__UP; //0x0200 | 0x0010 -> 0x210 ->  0010 0001 0000
    tcp->tp_c = TIMER_TAMANO_TP;
    tcp->to_c = TIMER_TAMANO_TO;

    for (n = tcp->tp_c ; n; --n)
    tcp->tp[n].banderas = 0x0;

    //for (n = tcp->to_c ; n; --n)
    for(n=0;n<tcp->to_c;n++)
    tcp->to[n].banderas = 0x0;


}




/*
void Timer_Process(timer_control_t *tcp)//Interfaz del main
{
    char n;
    __no_operation();
    if(TA0CTL&TAIFG)
    {
        TA0CTL &= ~TAIFG;

        // Revisa la tabla de per�odos y procesa los que est�n activos
        //for (n = tcp->tp_c ; n; n--)
        for (n=0; n < tcp->tp_c ; n++)
        if (tcp->tp[n].banderas & TIMER_ACTIVO)
        {
            tcp->tp[n].contador --;
            if ( (tcp->tp[n].contador)==0 )
            {
                tcp->tp[n].banderas |= TIMER_FINALIZADO;
                tcp->tp[n].contador = tcp->tp[n].valor;
            };

        };

        // Revisa la tabla de timeouts y procesa los que est�n activos
        //for (n = tcp->to_c; n; --n)
        for (n=0; n < tcp->to_c ; n++)
        if (tcp->to[n].banderas & TIMER_ACTIVO)
        {
            tcp->to[n].contador --;
            if ( tcp->to[n].contador ==0 )
            {
                tcp->to[n].banderas |= TIMER_FINALIZADO;
                tcp->to[n].banderas &= ~TIMER_ACTIVO ;
            };
        };
    }

}
*/

unsigned char Timer_Consulta_TO(timer_control_t *tcp, unsigned char to)
{
    if(tcp->to[to].banderas & TIMER_FINALIZADO)
    {
        tcp->to[to].banderas &= ~(TIMER_FINALIZADO);
        return 0x1;
    }
    return 0x0;
}

unsigned char Timer_Consulta_TP(timer_control_t *tcp, unsigned char tp)
{

    if(tcp->tp[tp].banderas & TIMER_FINALIZADO)
    {
        tcp->tp[tp].banderas &= ~(TIMER_FINALIZADO);
        return 0x1;
    }
    return 0x0;
}
void Timer_setup_TO(timer_control_t *tcp, unsigned char to, unsigned int valor)
{
    tcp->to[to].valor =  valor;
    tcp->to[to].contador = valor;
    tcp->to[to].banderas |= TIMER_ACTIVO;
}
void Timer_setup_TP(timer_control_t *tcp, unsigned char tp, unsigned int valor)
{

    tcp->tp[tp].valor =  valor;
    tcp->tp[tp].contador = valor;
    tcp->tp[tp].banderas |= TIMER_ACTIVO;
}
void Timer_apagar_TP(timer_control_t *tcp, unsigned char tp)
{
    tcp->tp[tp].banderas &= ~(TIMER_ACTIVO);
}
