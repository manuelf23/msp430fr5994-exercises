/*
 * timer.h
 *
 *  Created on: Mar 19, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <msp430.h>

#define TIMER_TAMANO_TO 3
#define TIMER_TAMANO_TP 2

#define TIMER_ACTIVO 0x01
#define TIMER_FINALIZADO 0x02

typedef struct t_control_t t_control_t;
struct t_control_t
{
    unsigned char banderas;
    unsigned int valor;
    unsigned int contador;
};


typedef struct timer_control_t timer_control_t;
struct timer_control_t
{
    unsigned char to_c; //cantidad de tmout configurados
    t_control_t to[TIMER_TAMANO_TO];

    unsigned char tp_c; //cantidad de periodos configurados
    t_control_t tp[TIMER_TAMANO_TP];
};

void Timer_init(timer_control_t *tcp);
void Timer_Process(timer_control_t *tcp);//Interfaz del main
unsigned char Timer_Consulta_TO(timer_control_t *tcp, unsigned char to);
unsigned char Timer_Consulta_TP(timer_control_t *tcp, unsigned char tp);
void Timer_setup_TO(timer_control_t *tcp, unsigned char to, unsigned int valor);
void Timer_setup_TP(timer_control_t *tcp, unsigned char tp, unsigned int valor);
void Timer_apagar_TP(timer_control_t *tcp, unsigned char tp);



#endif /* TIMER_H_ */
