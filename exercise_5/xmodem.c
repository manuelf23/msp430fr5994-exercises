/*
 * xmodem.c
 *
 *  Created on: Mar 25, 2020
 *      Author: manuelfelipegarciarincon
 */
#include <xmodem.h>
void xmodem_init(xmodem_control_t *xmodem, timer_control_t *tcp, rx_control_t *rcp, datos_control_t *dcp)
{
    xmodem->tcp = tcp;
    xmodem->rcp = rcp;
    xmodem->dcp = dcp;
    xmodem->ffpaquete = 0;
    xmodem->p_duplicado = 0;
    xmodem->secuencia = 0;
    xmodem->state = INI_CON;
    xmodem->contador_errores = 0;
    xmodem->ffpaquete = 0; //flag fin de paquete

    xmodem->buffer_uso = 0;
    xmodem->buffer_llenado = 0;
    xmodem->buffer_vaciado = 0;
}
void xmodem_process(xmodem_control_t *xmodem)
{
    char dato_rx;

    switch(xmodem->state)
    {
        case INI_CON: // ESTADO 0
            rx_enviar(NAK);
            Timer_setup_TO(xmodem->tcp, TIMEOUT_0, TM1S);
            xmodem->state = CONX_VALIDA;
            break;

        case CONX_VALIDA: // ESTADO 1
            //char dato_rx;
            if(rx_dato_disponible(xmodem->rcp))
            {
                __disable_interrupt();
                dato_rx = rx_leer_dato(xmodem->rcp);
                __enable_interrupt();
            }
            //dato_rx = rx_leer_dato(xmodem->rcp);

            if (dato_rx == SOH)
            {
                xmodem_guardar_dato(xmodem, dato_rx);
                Timer_setup_TO(xmodem->tcp, TIMEOUT_0, TM1S);
                xmodem->ffpaquete = 0;
                xmodem->state = VALIDACION;
            }
            else if(Timer_Consulta_TO(xmodem->tcp, TIMEOUT_0))
            {
                xmodem->state = INI_CON;
            }
                break;
        case VALIDACION: // ESTADO 2
            __no_operation();

            if(xmodem->buffer_uso < DATOS_XMODEM_BUFFER_TAMANO) // este if valida si NO esta lleno el buffer
            {
                // si entra va llenando el buffer

                        if(rx_dato_disponible(xmodem->rcp))
                        {
                            __disable_interrupt();
                            dato_rx = rx_leer_dato(xmodem->rcp);
                            __enable_interrupt();
                            xmodem_guardar_dato(xmodem, dato_rx);
                            Timer_setup_TO(xmodem->tcp, TIMEOUT_0, TM1S);
                        }
                        else if(Timer_Consulta_TO(xmodem->tcp, TIMEOUT_0)) // No llego ni SOH ni EOT
                        {
                            xmodem->state = RESPUESTA;
                            rx_enviar(NAK);
                            Timer_setup_TO(xmodem->tcp, TIMEOUT_0, T10S);
                            xmodem->contador_errores ++;

                        };

                        //dato_rx = rx_leer_dato(xmodem->rcp);
                        //xmodem_guardar_dato(xmodem, dato_rx);



            }
            else if (xmodem_paquete_valido(xmodem) == VALIDO)
            {
                if(xmodem->p_duplicado && xmodem->secuencia_copiada == xmodem->secuencia)
                {
                    rx_enviar(ACK);
                    xmodem->p_duplicado = 0;
                    Timer_setup_TO(xmodem->tcp, TIMEOUT_0, T10S);
                    xmodem->state = RESPUESTA;

                }
                else
                {
                    datos_copiar_paquete(xmodem->dcp, &xmodem->xmodem_buffer[3]);
                    xmodem->secuencia_copiada = xmodem->secuencia;
                    xmodem->contador_errores = 0;
                    xmodem->buffer_uso = 0;
                    xmodem->buffer_llenado = 0;
                    xmodem->buffer_vaciado = 0;
                    xmodem->state = ESPERA_PDATOS;
                }
            }
            else
            {
                xmodem->contador_errores ++;
                rx_enviar(NAK);
                Timer_setup_TO(xmodem->tcp, TIMEOUT_0, T10S);
                xmodem->buffer_uso = 0;
                xmodem->buffer_llenado = 0;
                xmodem->buffer_vaciado = 0;
                xmodem->state = RESPUESTA;
            }
                break;
        case ESPERA_PDATOS: // ESTADO 3
            if (datos_esta_libre(xmodem->dcp))
            {
                rx_enviar(ACK);
                Timer_setup_TO(xmodem->tcp, TIMEOUT_0, T10S);
                xmodem->state = RESPUESTA;
            }
            break;
        case RESPUESTA: // ESTADO 4
            if(xmodem->contador_errores >= 10)
            {
                xmodem->contador_errores = 0;
                xmodem->state = INI_CON;
                break;
            }


            if(rx_dato_disponible(xmodem->rcp))
            {
                __disable_interrupt();
                dato_rx = rx_leer_dato(xmodem->rcp);
                __enable_interrupt();
                if (dato_rx == SOH)
                {
                    xmodem_guardar_dato(xmodem, dato_rx);
                    xmodem->state = VALIDACION;
                }
                else if(dato_rx == EOT)
                {
                    rx_enviar(ACK);
                    datos_fin_paquete(xmodem->dcp);
                    xmodem->contador_errores = 0;
                    xmodem->ffpaquete = 1;
                    xmodem->buffer_uso = 0;
                    xmodem->buffer_llenado = 0;
                    xmodem->buffer_vaciado = 0;
                    xmodem->state = INI_CON;

                }
            }
            //dato_rx = rx_leer_dato(xmodem->rcp);

            else if(Timer_Consulta_TO(xmodem->tcp, TIMEOUT_0)) // No llego ni SOH ni EOT
            {
                rx_enviar(NAK);
                Timer_setup_TO(xmodem->tcp, TIMEOUT_0, T10S);
                xmodem->contador_errores ++;

            }
            break;

    }
}
char xmodem_paquete_valido(xmodem_control_t *xmodem)
{
    char a, csum, csum_calculado, seq, seq_a;
    if(xmodem->xmodem_buffer[0] == SOH)
    {
        seq = xmodem->xmodem_buffer[1];
        seq_a = 255 - seq;

        if(seq_a  == xmodem->xmodem_buffer[2])
        {
         csum  = xmodem->xmodem_buffer[131];
         csum_calculado=0;
         for(a = 3; a < DATOS_XMODEM_BUFFER_TAMANO - 1 ; a ++)
         {
             csum_calculado += xmodem->xmodem_buffer[a];
         }
         __no_operation();
         //return VALIDO;
         if (csum == csum_calculado)
         {
             if(seq == xmodem->secuencia)
             {
                 xmodem->p_duplicado = 1;
             }
             else
             {
                 xmodem->secuencia = seq;
             }
             return VALIDO;
         }
         //calcular suma
        }

    }
    return 0;
}

void xmodem_guardar_dato(xmodem_control_t *xmodem, char dato)
{

    xmodem->xmodem_buffer[xmodem->buffer_llenado] = dato;
    xmodem->buffer_uso ++;
    xmodem->buffer_llenado ++;
    if(xmodem->buffer_llenado == DATOS_XMODEM_BUFFER_TAMANO)
    {
        xmodem->buffer_llenado = 0;
    }
}
char xmodem_finalizo_paquete(xmodem_control_t *xmodem)
{
    return xmodem->ffpaquete ;
}
