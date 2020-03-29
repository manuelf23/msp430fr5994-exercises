/*
 * datos.c
 *
 *  Created on: Mar 28, 2020
 *      Author: manuelfelipegarciarincon
 */

#include <datos.h>
void datos_init(datos_control_t *dcp, timer_control_t *tcp, dy_control_t *dycp)
{
    dcp->tcp = tcp;
    dcp->dycp = dycp;

    dcp->dato_mostrar = '-';
    dcp->flag_2hz = 0;
    dcp->buffer_uso = 0;
    dcp->buffer_llenado = 0;
    dcp->buffer_vaciado = 0;
    Timer_setup_TP(dcp->tcp, TIMER_1, PERIODO); //timer para sacar datos a 120Hz
}
void datos_process(datos_control_t *dcp)
{
    if (Timer_Consulta_TP(dcp->tcp, TIMER_1) != 0)
    {
        if(dcp->buffer_uso > 0)
        {
            dcp->flag_2hz = 0;
            dcp->dato_mostrar = datos_sacar_dato(dcp);
            Timer_setup_TO(dcp->tcp, TIMEOUT_1, T20S);
            display_camb_blink(dcp->dycp, NO_BLINK);

        }
        else if(Timer_Consulta_TO(dcp->tcp, TIMEOUT_1) != 0 || dcp->flag_2hz == 1)
        {
            dcp->flag_2hz = 1;
            display_camb_blink(dcp->dycp, BLINK_2HZ);

        }
        else
        {
            display_camb_blink(dcp->dycp, BLINK_1HZ);
        }

        display_mostrar(dcp->dycp, dcp->dato_mostrar);
    }

}
void datos_copiar_paquete(datos_control_t *dcp, unsigned char *paquete)
{
    char a;
    unsigned char *puntero;
    puntero = paquete;
    for(a = 0; a < DATOS_DATOS_BUFFER_TAMANO; a ++)
        {
            dcp->datos_buffer[dcp->buffer_llenado] = *puntero;
            puntero ++;
            dcp->buffer_uso ++;
            dcp->buffer_llenado ++;
            if(dcp->buffer_llenado == DATOS_DATOS_BUFFER_TAMANO)
            {
                dcp->buffer_llenado = 0;
            }
        }
}
char datos_esta_libre(datos_control_t *dcp)
{
    if(dcp->buffer_uso == 0)
    {
        return 1;
    }
    return 0;
}
char datos_sacar_dato(datos_control_t *dcp)
{
    char dato ;
    dato = dcp->datos_buffer[dcp->buffer_vaciado];
    dcp->buffer_vaciado ++;
    dcp->buffer_uso --;
    if(dcp->buffer_vaciado == DATOS_DATOS_BUFFER_TAMANO)
    {
        dcp->buffer_vaciado = 0;
    }
    return dato;
}

void datos_fin_paquete(datos_control_t *dcp)
{
    dcp->dato_mostrar = '-';
    dcp->buffer_uso = 0;
    dcp->buffer_llenado = 0;
    dcp->buffer_vaciado = 0;
}
