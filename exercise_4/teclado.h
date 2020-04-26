/*
 * teclado.h
 *
 *  Created on: Apr 26, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef TECLADO_H_
#define TECLADO_H_

#include <msp430.h>
#include <timer.h>
#include <datos.h>

#define PERIODO_MUESTREO        60
#define TIMER_2                 2
#define NUM_TECLAS              2
#define TECLA_MENOS             0
#define TECLA_MAS               1

#define PUERTO_TECLA_MENOS      0x02
#define PUERTO_TECLA_MAS        0x04

typedef struct teclado_control_t teclado_control_t;
struct teclado_control_t
{
    timer_control_t *tcp;
    datos_control_t *dcp;
    char val_actual;
    char val_anterior;
    char contador;
    char val_puerto;
};

void teclado_init(teclado_control_t *tclp, timer_control_t *tcp, datos_control_t *dcp);
void teclado_process(teclado_control_t *tclp);

#endif /* TECLADO_H_ */
