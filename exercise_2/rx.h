/*
 * rx.h
 *
 *  Created on: Mar 6, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef RX_H_
#define RX_H_
#include <timer.h>
#include <msp430.h>

#define DATOS_RX_BUFFER_TAMANO  16
#define XON                     0x13
#define XOFF                    0x11
#define HWM                     11
#define LWM                     4
#define TMOUT_0                 0

typedef struct rx_control_t rx_control_t;
struct rx_control_t
{
    timer_control_t *tcp;

    unsigned char banderas;

    unsigned char rx_tamano;
    unsigned char rx_uso;
    unsigned char rx_llenado;
    unsigned char rx_vaciado;
    unsigned char rx_buffer[DATOS_RX_BUFFER_TAMANO];
};

void rx_init(rx_control_t *rcp, timer_control_t *tcp);

void rx_process(rx_control_t *rcp);

char rx_hay_dato(rx_control_t *rcp);

char rx_leer_dato(rx_control_t *rcp);

char rx_off_tout(rx_control_t *rcp);


#endif /* RX_H_ */
