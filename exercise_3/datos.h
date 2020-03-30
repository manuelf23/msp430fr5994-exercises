/*
 * datos.h
 *
 *  Created on: Mar 28, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef DATOS_H_
#define DATOS_H_

#include <msp430.h>
#include <timer.h>
#include <display.h>

#define DATOS_DATOS_BUFFER_TAMANO  128*2
#define TIMER_1                 1
#define TIMEOUT_1               1

#define PERIODO                 10 // 1/120Hz
#define T20S                    1200 * 20

typedef struct datos_control_t datos_control_t;
struct datos_control_t
{
    timer_control_t *tcp;
    dy_control_t *dycp;
    unsigned char dato_mostrar;
    unsigned char flag_2hz;
    unsigned char dato_fin_paquete;

    int buffer_uso;
    unsigned int buffer_llenado;
    unsigned int buffer_vaciado;
    unsigned char datos_buffer[DATOS_DATOS_BUFFER_TAMANO];
};

void datos_init(datos_control_t *dcp, timer_control_t *tcp, dy_control_t *dycp);
void datos_process(datos_control_t *dcp);
void datos_copiar_paquete(datos_control_t *dcp, unsigned char *paquete);
char datos_esta_libre(datos_control_t *dcp);
char datos_sacar_dato(datos_control_t *dcp);
void datos_fin_paquete(datos_control_t *dcp);

#endif /* DATOS_H_ */
