/*
 * rx.h
 *
 *  Created on: Mar 24, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef RX_H_
#define RX_H_

#include <timer.h>
#include <msp430.h>

#define DATOS_RX_BUFFER_TAMANO  22


typedef struct rx_control_t rx_control_t;
struct rx_control_t
{


    unsigned char rx_tamano;
    unsigned char rx_uso;
    unsigned char rx_llenado;
    unsigned char rx_vaciado;
    unsigned char rx_buffer[DATOS_RX_BUFFER_TAMANO];
};

void rx_init(rx_control_t *rcp);

void rx_process(rx_control_t *rcp);

char rx_leer_dato(rx_control_t *rcp);

void rx_enviar(char dato);
char rx_dato_disponible(rx_control_t *rcp);


#endif /* RX_H_ */
