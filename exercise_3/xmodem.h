/*
 * xmodem.h
 *
 *  Created on: Mar 25, 2020
 *      Author: manuelfelipegarciarincon
 */

#ifndef XMODEM_H_
#define XMODEM_H_

#include <rx.h>
#include <timer.h>
#include <datos.h>
#include <msp430.h>

#define INI_CON         0
#define CONX_VALIDA     1
#define VALIDACION      2
#define ESPERA_PDATOS   3
#define RESPUESTA       4

#define DATOS_XMODEM_BUFFER_TAMANO  133

#define NO_VALIDO       0X00
#define VALIDO          0X01
#define FINALIZO        0X02

#define S1              1200 // 1 segundo
#define T10S            S1 * 10
#define TM1S            S1 / 2 //medio segundo
#define TIMEOUT_0       0

#define SOH             0x01
#define EOT             0x04
#define ACK             0x06
#define NAK             0x15


typedef struct xmodem_control_t xmodem_control_t;
struct xmodem_control_t
{
    timer_control_t *tcp;
    rx_control_t *rcp;
    datos_control_t *dcp;

    unsigned char state;
    unsigned char contador_errores;
    unsigned char ffpaquete; //flag fin de paquete


    unsigned char buffer_uso;
    unsigned char buffer_llenado;
    unsigned char buffer_vaciado;
    unsigned char xmodem_buffer[DATOS_XMODEM_BUFFER_TAMANO];
};
void xmodem_init(xmodem_control_t *xmodem, timer_control_t *tcp, rx_control_t *rcp, datos_control_t *dcp);
void xmodem_process(xmodem_control_t *xmodem);
void xmodem_guardar_dato(xmodem_control_t *xmodem, char dato);
char xmodem_paquete_valido(xmodem_control_t *xmodem);
char xmodem_finalizo_paquete(xmodem_control_t *xmodem);


#endif /* XMODEM_H_ */
