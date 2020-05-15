/*
 * corrutinas.h
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef CORRUTINAS_H_
#define CORRUTINAS_H_
#include <stdint.h>
#define NULL 0

typedef void Corrutina(volatile void *parametro);
typedef struct Tcb Tcb;
struct Tcb
  {
    uint32_t *task_sp;
  };

void Inicie_cr (Tcb *tcb, uint32_t *task_stack, Corrutina *rutina, void *parametro);
void Ejecute_cr (Tcb *tcb);

#endif /* CORRUTINAS_H_ */
