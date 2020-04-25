/*
 * display.h
 *
 *  Created on: Mar 19, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include <timer.h>
#include <msp430.h>
#define CONTADOR     4
#define INTENSIDAD   3
#define TIMER_0      0
#define NO_BLINK     0
#define BLINK_1HZ    1
#define BLINK_2HZ    2


typedef struct dy_control_t dy_control_t;
struct dy_control_t
{
    timer_control_t *tcp;
    unsigned char blink;
    unsigned int contador;
    unsigned char num_display;
    unsigned char valor_display_A;
    unsigned char valor_display_B;
    unsigned char valor_display_C;
};

void display_process(dy_control_t *dycp);

void display_init(dy_control_t *dycp, timer_control_t *tcp);

void display_mostrar(dy_control_t *dycp, unsigned char val);

void display_camb_blink(dy_control_t *dycp, unsigned char blink);




#endif /* DISPLAY_H_ */
