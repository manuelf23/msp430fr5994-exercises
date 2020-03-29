/*
 * display.c
 *
 *  Created on: Mar 19, 2020
 *      Author: manuelfelipegarciarincon
 */
#include <display.h>
#include <msp430.h>

void display_init(dy_control_t *dycp, timer_control_t *tcp)
{
    P3DIR = 0xFF;
    P3OUT = 0x00;
    P3OUT = ~0x02;
    P1DIR = 0x38; // se habilito el p1.3, p1.4, p1.5 como salida
    P1OUT = 0x00;
    //P1OUT = 0x08;
    dycp->tcp = tcp;
    dycp->blink = NO_BLINK;
    dycp->contador = 0;
    dycp->num_display = 0;
    dycp->valor_display_A = 16;
    dycp->valor_display_B = 16;
    dycp->valor_display_C = 16;

    Timer_setup_TP(dycp->tcp, TIMER_0, 1); //timer para refresco de display a 1200Hz

}

void display_process(dy_control_t *dycp)
{
    unsigned char d_tabla[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0xF7,0x7C,0x39,0x5E,0x79,0x71,0x40};
    //P3OUT &= ~0x5B;
   if (Timer_Consulta_TP(dycp->tcp, TIMER_0) != 0)
    {
       __no_operation();
        if (dycp->num_display == 3)
        {
            dycp->num_display = 0;
        }

        switch(dycp->blink)
           {
               case 0:
                   dycp->contador = 0 ;
                   if(dycp->num_display == 0)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display_A]);
                       P1OUT = 0x08;
                   }
                   else if (dycp->num_display == 1)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display_B]);
                       P1OUT = 0x10;
                   }
                   else
                   {
                      P3OUT = ~(d_tabla[dycp->valor_display_C]);
                      P1OUT = 0x20;
                   }
                   break;
               case 1:
                   if (dycp->contador >= 1200)
                   {
                       dycp->contador =0;
                   }

                   if (dycp->contador < 600)
                   {
                       if(dycp->num_display == 0)
                         {
                             P3OUT = ~(d_tabla[dycp->valor_display_A]);
                             P1OUT = 0x08;
                         }
                         else if (dycp->num_display == 1)
                         {
                             P3OUT = ~(d_tabla[dycp->valor_display_B]);
                             P1OUT = 0x10;
                         }
                         else
                         {
                            P3OUT = ~(d_tabla[dycp->valor_display_C]);
                            P1OUT = 0x20;
                         }
                   }
                   else
                   {
                       __no_operation();
                      P1OUT = 0x00;
                   }
                   dycp->contador ++;
                   break;
               case 2:
                   if (dycp->contador >= 600)
                  {
                      dycp->contador =0;
                  }

                  if (dycp->contador < 300)
                  {
                      if(dycp->num_display == 0)
                        {
                            P3OUT = ~(d_tabla[dycp->valor_display_A]);
                            P1OUT = 0x08;
                        }
                        else if (dycp->num_display == 1)
                        {
                            P3OUT = ~(d_tabla[dycp->valor_display_B]);
                            P1OUT = 0x10;
                        }
                        else
                        {
                           P3OUT = ~(d_tabla[dycp->valor_display_C]);
                           P1OUT = 0x20;
                        }
                  }
                  else
                  {
                      __no_operation();
                     P1OUT = 0x00;
                  }
                  dycp->contador ++;

                  break;
           }
        dycp->num_display ++;


    }
}

void display_mostrar(dy_control_t *dycp, unsigned char val)
{
    if(val != '-')
    {
        dycp->valor_display_A = val / 100;
        dycp->valor_display_B = (val - (dycp->valor_display_A * 100)) / 10;
        dycp->valor_display_C = (val - (dycp->valor_display_A * 100) - (dycp->valor_display_B * 10));
    }
    else
    {
        dycp->valor_display_A = 16;
        dycp->valor_display_B = 16;
        dycp->valor_display_C = 16;
        display_camb_blink(dycp, NO_BLINK);
    }

}


void display_camb_blink(dy_control_t *dycp, unsigned char blink)
{
    dycp->blink = blink;

}
