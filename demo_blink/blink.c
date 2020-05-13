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

#define LONG_STACK_BLINK        50

typedef struct Tcb Tcb;
struct Tcb
  {
  void *sp;
  };
typedef void Corrutina(volatile void *parametro);

Tcb *tcb_actual;

Tcb   tcb_blink,
      tcb_ppal;

char  stack_blink[LONG_STACK_BLINK];

void Inicie_cr (Tcb *tcb, void *sp, Corrutina *rutina, void *parametro);
void Ejecute_cr (Tcb *tcb);
void blink(void);



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    Inicie_cr(&tcb_blink, stack_blink + LONG_STACK_BLINK-1, blink, 0);
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
    static void *sp;

    // Guardar contexto (en el stack)
    asm (" push {R15}");
    asm (" push {R14}");
    asm (" push {R13}");
    asm (" push {R12}");
    asm (" push {R11}");
    asm (" push {R10}");
    asm (" push {R9}");
    asm (" push {R8}");
    asm (" push {R7}");
    asm (" push {R6}");
    asm (" push {R5}");
    asm (" push {R4}");
    asm (" push {R3}");
    asm (" push {R2}");

    // sp <- SP (en asembler)
    asm(" mov sp, SP"); //SP->R1
    tcb_actual->sp = sp;
    tcb_actual = tcb;
    sp = tcb_actual->sp;
    // SP <- sp (en asembler)
    asm (" mov R4, sp");
    asm (" mov SP,R4"); //SP->R1
    // Restaurar contexto (del stack)
    asm (" pop {R2}");
    asm (" pop {R3}");
    asm (" pop {R4}");
    asm (" pop {R5}");
    asm (" pop {R6}");
    asm (" pop {R7}");
    asm (" pop {R8}");
    asm (" pop {R9}");
    asm (" pop {R10}");
    asm (" pop {R11}");
    asm (" pop {R12}");
    asm (" pop {R13}");
    asm (" pop {R14}");
    asm (" pop {R15}");
}

#define INIT_SR            0        /* Valor inicial para el SR, principalmente por
                                       la mascara de interrupciones */
#define LONG_RESTO_CTX     5

void Inicie_cr (Tcb *tcb, void *sp, Corrutina *rutina, void *parametro)
{
    --(((void *) *) sp); /*SP está en la última posición ocupada del stack, por lo tanto se decrementa
                            para que quede en una posición libre, en donde se guardará el valor de parámetro*/
   /* ((char *) sp) -= sizeof(void *); -> otra forma de hacer lo mismo */
    *(((void*) *) sp) = parametro; /*Se guarda el valor de parámetro en lo apuntado por ((void*)*)sp */

    ((Corrutina**)sp) -= 2; /* Mueve el SP dos posiciones para poder guardar el PC inicial de la rutina
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

    *((Corrutina**)sp) = rutina; /*Se guarda el valor de la direccion de la corrutina en lo apuntado por (Corrutina**)sp */

    --((unsigned short *) sp);
    *((unsigned short *) sp) = INIT_SR;
/* Opcion: *((unsigned short *) sp) = inicio_sr; */
    ((unsigned short *) sp) -= LONG_RESTO_CTX;

   tcb->sp = sp;
}
