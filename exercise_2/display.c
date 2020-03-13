#include <display.h>
#include <msp430.h>



void display_init(dy_control_t *dycp, timer_control_t *tcp)
{
    P3DIR = 0xFF;
    P3OUT = 0x00;
    P3OUT = ~0x02;
    dycp->tcp = tcp;
    dycp->intensidad = 3;
    dycp->contador = 0;
    dycp->valor_display = 16;

    Timer_setup_TP(dycp->tcp, TIMER_0, 2); //timer para refresco de display + 80Hz

}

void display_process(dy_control_t *dycp)
{
    unsigned char d_tabla[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0xF7,0x7C,0x39,0x5E,0x79,0x71,0x40};
    //P3OUT &= ~0x5B;
   if (Timer_Consulta_TP(dycp->tcp, TIMER_0))
    {
        if (dycp->contador == 4)
        {
            dycp->contador = 0;
        }

        switch(dycp->intensidad)
           {
               case 1:
                   if(dycp->contador <= 1)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display]);
                   }
                   else
                   {
                       P3OUT = ~(0x00);
                   }
                   break;
               case 2:
                   if(dycp->contador <= 2)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display]);
                   }
                   else
                   {
                       P3OUT = ~(0x00);
                   }
                   break;
               case 3:
                   if(dycp->contador <= 3)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display]);
                   }
                   else
                   {
                       P3OUT = ~(0x00);
                   }
                   break;
               case 4:
                   if(dycp->contador <= 4)
                   {
                       P3OUT = ~(d_tabla[dycp->valor_display]);
                   }
                   else
                   {
                       P3OUT = ~(0x00);
                   }
                   break;
               default:
                   P3OUT = ~(0x00);
                   break;
           }
        dycp->contador ++;

    }
}

void display_mostrar(dy_control_t *dycp, unsigned char val)
{
    dycp->valor_display = val;

}


void display_camb_intensidad(dy_control_t *dycp, unsigned char intensidad)
{
    dycp->intensidad = intensidad;

}

