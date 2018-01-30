

#include "elecanisms.h"

int16_t main(void) {
    init_elecanisms();

    T2CON = 0x0030;         // set Timer2 period to 0.5s
    PR2 = 0x7A11;

    TMR2 = 0;               // set Timer2 count to 0
    IFS0bits.T2IF = 0;      // lower Timer2 interrupt flag
    T2CONbits.TON = 1;      // turn on Timer2

    LED2 = ON;

    while (1) {
        if (IFS0bits.T2IF == 1) {
            IFS0bits.T2IF = 0;      // lower Timer1 interrupt flag
            LED2 = ON;           // toggle LED2
            //LED3 = !LED3;
        }

    }
}






/*
#include <time.h>
#include "elecanisms.h"
//#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  init_elecanisms();

  time_t then;
  time_t now;

  now = time(NULL);
  double diff = difftime(now, then);
  LED1 = OFF;
  LED2 = OFF;
  LED3 = OFF;


while (1) {
  now = time(NULL);
  double diff = difftime(now, then);
  if (diff > 5) {
    LED1 = ON;
  }
  if(diff < 5){
    LED2 = ON;
  }
}

}
*


/*
#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
//#include "ui.h"
#include <timer.h>

uint16_t state, counter;

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    timer_setPeriod(&timer1, 0.5);

    state = ZZERO;
    LED1 = OFF;
    LED2 = OFF;
    LED3 = OFF;

    while(1){
        switch (state){
          case ZZERO:
            if (D0 == 0) {
              timer_start(&timer1);
              LED1 = ON;
              counter = 0;
            }
          case OONE:
            if (timer_flag(&timer1)) {
              timer_lower(&timer);
              LED2 = !LED2;
              counter ++ ;
            }
            if(counter == 20){
              LED3 = ON;
            }
        }
    }
    */
