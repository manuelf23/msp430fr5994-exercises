/*
 * toggle.h
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef TOGGLE_H_
#define TOGGLE_H_

#include <corrutinas.h>

typedef struct toggle_control_t toggle_control_t;
struct toggle_control_t
{
    //Tcb tcb_toggle;
    //Tcb tcb_ppal;
    Tcb tcb_ppal;
    double val;
};
void toggle_init(toggle_control_t *tgcp);
void toggle_process(toggle_control_t *tgcp);


#endif /* TOGGLE_H_ */
