/*
 * blink.h
 *
 *  Created on: May 15, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef BLINK_H_
#define BLINK_H_

#include <corrutinas.h>

typedef struct blink_control_t blink_control_t;
struct blink_control_t
{
    int val;
};

void blink_init(blink_control_t *bcp);
void blink_process(blink_control_t *bcp);


#endif /* BLINK_H_ */
