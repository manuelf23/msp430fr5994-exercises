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

typedef struct Tcb Tcb;
struct Tcb
  {
    uint16_t *task_sp;
  };
typedef void Corrutina(volatile void *parametro);

static uint16_t *task_sp;

Tcb *tcb_actual;

Tcb   tcb_blink,
      tcb_ppal;
__no_operation();
static uint16_t  stack_blink[LONG_STACK_BLINK]= {0};

//void Inicie_cr (Tcb *tcb, uint16_t *sp, Corrutina *rutina, void *parametro);
void Inicie_cr (Tcb *tcb, uint16_t *task_stack, Corrutina *func, uint16_t stack_bytes);
void Ejecute_cr (Tcb *tcb);
void blink(void);



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    Inicie_cr(&tcb_blink, stack_blink, blink, LONG_STACK_BLINK);
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
        Ejecute_cr (&tcb_ppal);
    }
}

void Ejecute_cr (Tcb *tcb)
{
    // static uint16_t *task_sp;

    // Guardar contexto (en el stack)
    asm (" POPM.A #5,R15 \n\t");
    asm (" pushm.a #12, r15 \n\t");
    // sp <- SP (en asembler)
    asm(" mov sp, task_sp \n\t"); //SP->R1
    tcb_actual->task_sp = task_sp;
    tcb_actual = tcb;
    task_sp = tcb_actual->task_sp;
    // SP <- sp (en asembler)
    //asm (" mov R4, sp");
    asm (" mov task_sp, sp \n\t"); //SP->R1
    // Restaurar contexto (del stack)
    asm (" popm.a #12, r15 \n\t");
}

#define INIT_SR            0        /* Valor inicial para el SR, principalmente por
                                       la mascara de interrupciones */
#define LONG_RESTO_CTX     5
#define NUM_REGISTERS      15

void Inicie_cr (Tcb *tcb, uint16_t *task_stack, Corrutina *func, uint16_t stack_bytes)
{
    /*--sp; SP está en la última posición ocupada del stack, por lo tanto se decrementa
                            para que quede en una posición libre, en donde se guardará el valor de parámetro*/
   /* ((char *) sp) -= sizeof(void *); -> otra forma de hacer lo mismo */
    /**sp = parametro; Se guarda el valor de parámetro en lo apuntado por ((void*)*)sp */

    /*(Corrutina**)sp --;
    (Corrutina**)sp --;  Mueve el SP dos posiciones para poder guardar el PC inicial de la rutina
                                               +--------------------------+
                                               :                          :
                                               |                          |
                                               +--------------------------+
                                               | Contexto                 |
                                               |                          |
                                               |                          |
                                               |                          |
                                               +--------------------------+
   (Despues de ejecutarse la instruccion) SP-> | PC inicial de la rutina  |
                                               +--------------------------+
                                               | PC de retorna de rutina  |   (el valor no importa porque no retorna)
                                               +--------------------------+
     (Antes de ejecutarse la instruccion) SP-> | Parametro                |
                                               +--------------------------+

    */

    /*  *((Corrutina**)sp) = rutina; Se guarda el valor de la direccion de la corrutina en lo apuntado por (Corrutina**)sp */

    /*--((unsigned short *) sp);
    *((unsigned short *) sp) = INIT_SR;*/
/* Opcion: *((unsigned short *) sp) = inicio_sr; */
    //((unsigned short *) sp) -= LONG_RESTO_CTX;

    uint16_t * stack = &task_stack[(stack_bytes/sizeof(uint16_t))-1];

        // First, store the 20-bit PC and the status
        // register. These will get popped as a last step
        // when returning from the timer interrupt.
        *(--stack) = (uint16_t)func;
        *(--stack) = 0x0; // Only set GIE in the status
                          // register we want to start with
                          // Upper 4 bits of PC are also stored
                          // here but are zeroed.

        int reg;
        for(reg=4; reg<NUM_REGISTERS; reg++) {
            // Start at register 4, since
            // R0 == PC
            // R1 == SP
            // R2 == SR
            // R3 is reserved
            // and we load these separately

            // Emulate PUSHX, and save two words
            // per 20-bit register
            *(--stack) = 0x0;
            *(--stack) = 0x0;
        }

   tcb->task_sp = (void *)stack;;
}
