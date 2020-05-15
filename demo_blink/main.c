#include <msp430.h>
#include <stdint.h>
#include <blink.h>
#include <toggle.h>
#define LONG_STACK_BLINK        50


Tcb tcb_blink, tcb_ppal, tcb_toggle;
blink_control_t bcp;
toggle_control_t tgcp;
extern Tcb *tcb_actual;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    blink_init(&bcp, &tcb_ppal);
    toggle_init(&tgcp, &tcb_ppal);

    static uint32_t  stack_blink[LONG_STACK_BLINK]= {0};
    static uint32_t  stack_toggle[LONG_STACK_BLINK]= {0};

    Inicie_cr(&tcb_blink, stack_blink + LONG_STACK_BLINK, blink_process, NULL);
    Inicie_cr(&tcb_toggle, stack_toggle + LONG_STACK_BLINK, toggle_process, &tgcp);

    tcb_actual=&tcb_ppal;

    while(1){
        Ejecute_cr (&tcb_blink);
        Ejecute_cr (&tcb_toggle);
    }
}

