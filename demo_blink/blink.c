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

#define LONG_STACK_BLINK        1024/2
#define NUM_REGISTERS      16
#define NULL 0

typedef struct Tcb Tcb;
struct Tcb
  {
    uint32_t *task_sp;
  };

Tcb *tcb_actual;
Tcb   tcb_blink,
      tcb_ppal;

typedef void Corrutina(volatile void *parametro);
static uint32_t sp_local;
static uint32_t *task_sp;
//static volatile void * volatile task_sp;
static uint32_t  stack_blink[LONG_STACK_BLINK]= {0};
static uint32_t *tam_stack_blink;
//void Inicie_cr (Tcb *tcb, uint16_t *sp, Corrutina *rutina, void *parametro);
void Inicie_cr (Tcb *tcb, uint32_t *task_stack, Corrutina *rutina, void *parametro);
void Ejecute_cr (Tcb *tcb);
void blink(void);



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    P1OUT &= ~0x01;
    tam_stack_blink = stack_blink + LONG_STACK_BLINK;
    Inicie_cr(&tcb_blink, stack_blink + LONG_STACK_BLINK, blink, NULL);
    tcb_actual=&tcb_ppal;

    for(;;) {
        volatile unsigned int i;            // volatile to prevent optimization

        //blink();
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
        Ejecute_cr (&tcb_ppal);
    }
}

void Ejecute_cr (Tcb *tcb)
{
    //static uint16_t *task_sp;
    //static volatile void * volatile task_sp;
    // Guardar contexto (en el stack)
    asm(" push.w sr");
    /*asm(" push.w r15");
    asm(" push.w r14");
    asm(" push.w r13");
    asm(" push.w r12");
    asm(" push.w r11");
    asm(" push.w r10");
    asm(" push.w r9");
    asm(" push.w r8");
    asm(" push.w r7");
    asm(" push.w r6");
    asm(" push.w r5");
    asm(" push.w r4");*/

    asm(" pushm.a #12, r15"); /*guarda en el stack 12 registros empezando desde el R15 y va decrementando
                                      R15, R14, R13, R12, R11, R10, R9, R8, R7, R6, R5, R4
                                      SP es decrementado en (n × 2) despues de realizar la operación*/


    /*asm(" mov.w   &task_sp, r12"); // MOV src,dst / src → dst /&=Absolute address
    asm(" mov.w   sp, 0( r12 )");
    */
    asm(" mov.a   sp, &task_sp");
    tcb_actual->task_sp = task_sp;
    tcb_actual = tcb;
    task_sp = tcb_actual->task_sp;
    asm(" mov.a   &task_sp, sp");
    /*
    asm(" mov.w   &task_sp, r12");
    asm(" mov.w  @r12, sp");    //@=Register indirect addressing
    */


    asm(" popm.a    #12, r15"); /*Reestaura registros de 16-bits desde el stack hacia la CPU empezando desde el R15 - 12
                                  y va decrementando
                                  R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
                                  SP es incrementado en (n × 2) despues de realizar la operación*/

    /*asm(" pop.w r4");
    asm(" pop.w r5");
    asm(" pop.w r6");
    asm(" pop.w r7");
    asm(" pop.w r8");
    asm(" pop.w r9");
    asm(" pop.w r10");
    asm(" pop.w r11");
    asm(" pop.w r12");
    asm(" pop.w r13");
    asm(" pop.w r14");
    asm(" pop.w r15");*/
    asm(" nop");
    asm(" pop.a sr");
    asm(" nop");
    //asm(" pop.w PC");
    //asm(" nop");
    //asm (" RETI \n\t");

}

void Inicie_cr (Tcb *tcb, uint32_t *task_stack, Corrutina *rutina, void *parametro)
{

    __no_operation();
    __no_operation();
    //task_stack = &task_stack[(LONG_STACK_BLINK/sizeof(uint16_t))-1];
    task_stack -= 1;
    *(--task_stack) = (uint32_t)rutina;
    sp_local = (uint32_t)rutina;
    //task_stack -= sizeof(Corrutina *); //decrementa dos posiciones
    //*((Corrutina**)task_stack) = rutina;
    *(--task_stack) = GIE; //Se guarda el SR
    task_stack -= 1; //Me muevo a R3
    //task_stack -= 1; //Me muevo a R4
    //*(task_stack) = 0x4444;
    *(--task_stack) = 0x00; //R15
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R14
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R13
    //task_stack -= 1;
    *((void**)task_stack) = parametro;
    //*(--task_stack) = (uint16_t)parametro; //R12
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R11
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R10
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R9
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R8
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R7
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R6
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R5
    //task_stack -= 1;
    *(--task_stack) = 0x00; //R4

    /*
    int reg;
    for(reg=4; reg<12; reg++)
    {
        *(--task_stack) = 0x00;
    }
    task_stack -= 1; // Me muevo a R12
    *((void**)task_stack) = parametro; //Le asigno valor a R12

    for(reg=13; reg<16; reg++)
    {
        *(--task_stack) = 0x00;
    }
    */
    tcb->task_sp = task_stack;
}
