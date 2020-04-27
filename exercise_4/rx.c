/*
 * rx.c
 *
 *  Created on: Mar 24, 2020
 *      Author: manuelfelipegarciarincon
 */


#include <rx.h>
#include <msp430.h>

void rx_init(rx_control_t *rcp)
{
    rcp->rx_tamano = DATOS_RX_BUFFER_TAMANO;
    rcp->rx_uso = 0;
    rcp->rx_llenado = 0;
    rcp->rx_vaciado = 0;
    rcp->rx_full = 0;


    //-------------------------------------------------------------------------
    //------------------------- UART1 Config (9600) ---------------------------
    //-------------------------------------------------------------------------
    P6SEL1 &= ~(BIT0 | BIT1);
    P6SEL0 |= (BIT0 | BIT1);

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure USCI_A3 for UART mode
    UCA3CTLW0 = UCSWRST;                    // Put eUSCI in reset
    UCA3CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA3BRW = 208;                           // 8000000/16/9600
    UCA3MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA3CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
    UCA3IE |= UCRXIE;                       // Enable USCI_A3 RX interrupt


    //UCA1IE |= UCRXIE; //habilitar interrupcion
}

/*
void rx_process(rx_control_t *rcp)
{
    __no_operation();
    if((UCA3IFG & UCRXIFG) !=0)
    {
        //UCA3TXBUF = UCA3RXBUF;

        rcp->rx_buffer[rcp->rx_llenado] = UCA3RXBUF;
        rcp->rx_uso ++;
        rcp->rx_llenado ++;
        if(rcp->rx_llenado == DATOS_RX_BUFFER_TAMANO)
        {
            rcp->rx_llenado = 0;
        }


    }

}
*/


char rx_leer_dato(rx_control_t *rcp)
{
    char dato;

    dato = rcp->rx_buffer[rcp->rx_vaciado];
    rcp->rx_vaciado ++;
    rcp->rx_uso --;
    if(rcp->rx_vaciado == DATOS_RX_BUFFER_TAMANO)
    {
        rcp->rx_vaciado = 0;
    }
    /*if (rcp->rx_full)                   //la cola esta en estado de lleno?
    {
        rcp->rx_full = 0;
        UCA3IE |= UCRXIE;               // Como se acaba de sacar un dato de la cola etonces Habilitar interrupción de UART
    }*/
    return dato;
}

void rx_enviar(char dato)
{
    UCA3TXBUF = dato;
}

char rx_dato_disponible(rx_control_t *rcp)
{
    return rcp->rx_uso;
}
