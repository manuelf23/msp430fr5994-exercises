/*
 * 7_segmentos.h
 *
 *  Created on: Mar 6, 2020
 *      Author: manuel felipe garcia rincon
 */

#ifndef SCR_S_SEGMENTOS_S_SEGMENTOS_H_
#define SCR_S_SEGMENTOS_S_SEGMENTOS_H_
#include <timer.h>
#include <msp430.h>
#define CONTADOR     4
#define INTENSIDAD   3
#define TIMER_0      0


//const
//        {0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   , 8   , 9   , A   , B   , C   , D   , E   , F   , -}

typedef struct dy_control_t dy_control_t;
struct dy_control_t
{
    timer_control_t *tcp;
    unsigned char intensidad;
    unsigned char contador;
    unsigned char valor_display;
};

void display_process(dy_control_t *dycp);

void display_init(dy_control_t *dycp, timer_control_t *tcp);

void display_mostrar(dy_control_t *dycp, unsigned char val);

void display_camb_intensidad(dy_control_t *dycp, unsigned char intensidad);



#endif /* SCR_S_SEGMENTOS_S_SEGMENTOS_H_ */
