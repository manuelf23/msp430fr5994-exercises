//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Texas Instruments, Inc
//  July 2013
//***************************************************************************************

#include <msp430.h>
#include <stdint.h>

#define LONG_STACK_BLINK        50
#define NUM_REGISTERS      16
#define NULL 0

typedef struct Tcb Tcb;
struct Tcb
  {
    uint32_t *task_sp;
  };

Tcb *tcb_actual;
Tcb   tcb_blink,
      tcb_ppal,
      tcb_toggle;

typedef void Corrutina(volatile void *parametro);
static uint32_t *task_sp;

static uint32_t  stack_blink[LONG_STACK_BLINK]= {0};
static uint32_t  stack_toggle[LONG_STACK_BLINK]= {0};

void Inicie_cr (Tcb *tcb, uint32_t *task_stack, Corrutina *rutina, void *parametro);
void Ejecute_cr (Tcb *tcb);
void blink(void);
void toggle(int val);


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    P1OUT &= ~0x01;
    Inicie_cr(&tcb_blink, stack_blink + LONG_STACK_BLINK, blink, NULL);
    Inicie_cr(&tcb_toggle, stack_toggle + LONG_STACK_BLINK, toggle, 30000);
    tcb_actual=&tcb_ppal;

    for(;;) {
        volatile unsigned int i;            // volatile to prevent optimization
        Ejecute_cr (&tcb_blink);
        i = 10000;                          // SW Delay
        do i--;
        while(i != 0);
    }


}
void blink(void)
{
    while(1)
    {
        P1OUT ^= 0x01;
        //Ejecute_cr (&tcb_ppal);
        Ejecute_cr (&tcb_toggle);
    }
}

void toggle(int val)
{
    for(;;) {
            volatile unsigned int i;            // volatile to prevent optimization
            Ejecute_cr (&tcb_blink);
            i = val;                          // SW Delay
            do i--;
            while(i != 0);
        }
}

void Ejecute_cr (Tcb *tcb)
{
    //sufijo .a -> 20-bits
    //sufijo .w -> 16-bits

    // Guardar contexto (en el stack)
    asm(" push.a sr");
    asm(" pushm.a #12, r15"); /*guarda los 12 registros empezando desde el R15 y va decrementando.
                                  R15, R14, R13, R12, R11, R10, R9, R8, R7, R6, R5, R4.
                                  SP es decrementado en (n × 4) despues de realizar la operación*/
    asm(" mov.a   sp, &task_sp");
    tcb_actual->task_sp = task_sp;
    tcb_actual = tcb;
    task_sp = tcb_actual->task_sp;

    // Restaurar contexto
    asm(" mov.a   &task_sp, sp");
    asm(" popm.a    #12, r15"); /*Reestaura los registros empezando desde el R15-12->R4
                                  y va incrementando R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15.
                                  SP es incrementado en (n × 4) despues de realizar la operación*/
    asm(" nop");
    asm(" pop.a sr");
    asm(" nop");

}

void Inicie_cr (Tcb *tcb, uint32_t *task_stack, Corrutina *rutina, void *parametro)
{
    /*Este procesador tiene direccionamiento en memoria de 20-BITs,
     * por lo tanto se usa un int de 32bit para moverse y asignar las direciones*/


    task_stack -= 1; /* Se deja el espacio para el PC de retorno de la rutina,
                        cómo nuca va a retornar, no importa el valor*/

    *(--task_stack) = (uint32_t)rutina; // Se guarda el PC (PC inicial de la rutina)
    *(--task_stack) = GIE; //Se guarda el SR (Status Register)

    int reg;
    for(reg=15; reg>12; reg--)
    {
        *(--task_stack) = 0x00; //Se dejan en 0x00 los registros R15, R14, y R13
    }
    *(--task_stack) = (uint32_t)parametro; /*A R12 se le asigna el valor del parametro.
                                            Este procesador pasa los argumentos a funciones por registros,
                                            usando los registros R12, R13,R14 y R15.
                                            En este caso, cómo solo se va a asignar un parámetro,
                                            solo se va a usar el registro R12  */

    for(reg=12; reg>4; reg--)
    {
        *(--task_stack) = 0x00; //Se dejan en 0x00 los registros R11, R10, R9, R8, R7, R6, R5 Y R4
    }
    // El registro R3, está reservado, por lo tanto no se le asigna valor.
    tcb->task_sp = task_stack;
}
