/*
 * datos.c
 *
 *  Created on: 6/03/2020
 *      Author: kamilo
 */
#include <datos.h>




void datos_init(datos_control_t *dcp, timer_control_t *tcp, rx_control_t *rcp, dy_control_t *dycp)
{
    dcp->tcp = tcp;
    Timer_setup_TP(dycp->tcp, TIMER_1, 1); // 1->160Hz


    dcp->banderas = 0x00;

};

void datos_process(datos_control_t *dcp)
{
    unsigned char dato_leido;

    if(rx_off_tout(dcp->rcp))
    {

        display_camb_intensidad(dcp->dycp, 3);
        display_mostrar(dcp->dycp, 16);
        return;
    }
    if(dcp->banderas & PORC)
    {
        if(Timer_Consulta_TO(dcp->tcp, TIMOUT_2))
        {
            dcp->banderas &= ~(PORC);
            display_camb_intensidad(dcp->dycp, 3);
            return;
        }
        display_camb_intensidad(dcp->dycp, 0);
        return;
    }
    if(Timer_Consulta_TP(dcp->tcp, TIMER_1))
    {
        if(rx_hay_dato(dcp->rcp))
        {
            dato_leido = rx_leer_dato(dcp->rcp);
            if(dato_leido == '&')
            {
                dcp->banderas |= ANDP;
            }
            else if(dato_leido == '$')
            {
                dcp->banderas &= ~(ANDP);
            }
            else if (dato_leido == '%')
            {
                dcp->banderas |= PORC;

                Timer_setup_TO(dcp->tcp, TIMOUT_2, 320); //320->2s //activo timeout APAGAR DISPLAY
            }
            else if (dato_leido == '#')
            {
                dcp->banderas |= NUME;
                Timer_setup_TO(dcp->tcp, TIMOUT_1, 800); //800->5s
            }
            else
            {

                dato_leido &= 0x0F;
                if(dcp->banderas & ANDP)
                {
                    dato_leido = ~(dato_leido + 1);
                }
                //-------------------------------



                if(dcp->banderas & NUME)
                {
                    if(Timer_Consulta_TO(dcp->tcp, TIMOUT_1))
                    {
                        dcp->banderas &= ~(PORC);
                        display_camb_intensidad(dcp->dycp, 3);
                    }

                    display_camb_intensidad(dcp->dycp, 2);
                }
                display_mostrar(dcp->dycp, dato_leido);


            }


        }

    }




};
