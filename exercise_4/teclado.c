/*
 * teclado.c
 *
 *  Created on: Apr 26, 2020
 *      Author: manuelfelipegarciarincon
 */

#include <teclado.h>

void teclado_init(teclado_control_t *tclp, timer_control_t *tcp, datos_control_t *dcp)
{
    P4DIR &= ~0x06; // P4.1 y P4.2 como entradas

    tclp->dcp = dcp;
    tclp->tcp = tcp;
    tclp->val_actual = 0x00;
    tclp->val_anterior = 0x00;
    tclp->contador = TECLA_MENOS;
    tclp->val_puerto = 0x0;

    Timer_setup_TP(tclp->tcp, TIMER_2, PERIODO_MUESTREO); //timer para refresco de display a 1200Hz
    __no_operation();
}
void teclado_process(teclado_control_t *tclp)
{

    if (Timer_Consulta_TP(tclp->tcp, TIMER_2) != 0)
    {

        switch(tclp->contador)
        {
        case TECLA_MENOS:
            tclp->val_puerto = P4IN & PUERTO_TECLA_MENOS;
            __no_operation();
            if(tclp->val_puerto != 0)
            {
                tclp->val_actual |= tclp->val_puerto;
            }
            else
            {
                tclp->val_actual &= ~PUERTO_TECLA_MENOS;
            }


            if( ((tclp->val_actual & PUERTO_TECLA_MENOS) != 0) && ((tclp->val_anterior & PUERTO_TECLA_MENOS) == 0) )
            {
                datos_menos_freq(tclp->dcp);
                __no_operation();
                __no_operation();
            }


            tclp->val_anterior = tclp->val_actual;
            tclp->contador = TECLA_MAS;
            break;
        case TECLA_MAS:

            tclp->val_puerto = P4IN & PUERTO_TECLA_MAS;
            if(tclp->val_puerto)
            {
                tclp->val_actual |= tclp->val_puerto;
            }
            else
            {
                tclp->val_actual &= ~PUERTO_TECLA_MAS;
            }


            if(((tclp->val_actual & PUERTO_TECLA_MAS) != 0) && ((tclp->val_anterior & PUERTO_TECLA_MAS) == 0))
            {
                datos_mas_freq(tclp->dcp);
                __no_operation();
                __no_operation();
            }
            tclp->val_anterior = tclp->val_actual;
            tclp->contador = TECLA_MENOS;
            break;
        default:
            __no_operation();
            break;

        }

    }
}
