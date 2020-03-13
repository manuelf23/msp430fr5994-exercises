/*
 * datos.h
 *
 *  Created on: 6/03/2020
 *      Author: kamilo
 */



#ifndef DATOS_H_
#define DATOS_H_
#include <msp430.h>
#include <timer.h>
#include <rx.h>
#include <display.h>

#define DATOS_RX_BUFFER_TAMANO  16
#define TIMER_1                 1
#define TIMOUT_1                 0
#define TIMOUT_2                 1

#define PESO                    0x00 // se muestra sin complemento
#define ANDP                    0x01 // se complementa a1

#define PORC                    0x02 // Se apaga el display por 2s
#define NUME                    0x04 // Se baja la intensidad por 5s

typedef struct datos_control_t datos_control_t;
struct datos_control_t
{
    timer_control_t *tcp;
    rx_control_t *rcp;
    dy_control_t *dycp;

    unsigned char banderas;
};

void datos_init(datos_control_t *dcp, timer_control_t *tcp, rx_control_t *rcp, dy_control_t *dycp);
void datos_process(datos_control_t *dcp);

#endif /* DATOS_H_ */
