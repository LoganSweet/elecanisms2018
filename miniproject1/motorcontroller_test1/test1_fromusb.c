#include "elecanisms.h"
#include "usb.h"
#include <stdio.h>

#define READ_A0             0
#define READ_A1             1
#define SET_DUTY_VAL        2
#define GET_DUTY_VAL        3
#define GET_DUTY_MAX        4

void vendor_requests(void) {
    //WORD temp;
    //uint16_t i;
    switch (USB_setup.bRequest) {
      case READ_A0:
          temp.w = read_analog(A0_AN);
          BD[EP0IN].address[0] = temp.b[0];
          BD[EP0IN].address[1] = temp.b[1];
          BD[EP0IN].bytecount = 2;
          BD[EP0IN].status = UOWN | DTS | DTSEN;
          break;
      case READ_A1:
              temp.w = read_analog(A1_AN);
              BD[EP0IN].address[0] = temp.b[0];
              BD[EP0IN].address[1] = temp.b[1];
              BD[EP0IN].bytecount = 2;
              BD[EP0IN].status = UOWN | DTS | DTSEN;
              break;
        case SET_DUTY_VAL:
            OC1R = USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case GET_DUTY_VAL:
            temp.w = OC1R;
            BD[EP0IN].address[0] = temp.b[0];
            BD[EP0IN].address[1] = temp.b[1];
            BD[EP0IN].bytecount = 2;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case GET_DUTY_MAX:
            temp.w = OC1RS;
            BD[EP0IN].address[0] = temp.b[0];
            BD[EP0IN].address[1] = temp.b[1];
            BD[EP0IN].bytecount = 2;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        default:
            USB_error_flags |= REQUEST_ERROR;
    }
}


void movemotor(void){
  uint8_t *RPOR, *RPINR;
  D8_DIR = OUT;      // configure D8 to be a digital output
  D8 = 0;            // set D8 low
  RPOR = (uint8_t *)&RPOR0;       // set RPOR to point at the reg for RPOR0,
  RPINR = (uint8_t *)&RPINR0;         // which it will pretend is insugned 8 bit int

  __builtin_write_OSCCONL(OSCCON & 0xBF);
  RPOR[D8_RP] = OC1_RP;          // connect the OC1 module output to pin D13
  __builtin_write_OSCCONL(OSCCON | 0x40);
                       // configure OC1 module to use the peripheral clock (i.e., FCY, OCTSEL<2:0> = 0b111)
  OC1CON1 = 0x1C06;    //   and to operate in edge-aligned PWM mode (OCM<2:0> = 0b110)
  OC1CON2 = 0x001F;   // configure OC1 module to syncrhonize to itself (i.e., OCTRIG = 0 and SYNCSEL<4:0> = 0b11111)
  OC1RS = (uint16_t)(FCY / 1e3 - 1.);     // configure period register to get a frequency of 1kHz
  OC1R = OC1RS >> 2;                      // configure duty cycle to 25% (i.e., period / 4)
  OC1TMR = 0;                             // set OC1 timer count to 0
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/*
  uint8_t *RPOR1, *RPINR1;
  D7_DIR = OUT;
  D7 = 0;
  RPOR2 = (uint8_t *)&RPOR1;
  RPINR2 = (uint8_t *)&RPINR1;

  __builtin_write_OSCCONL(OSCCON & 0xBF);
  RPOR[D7_RP] = OC2_RP;
  __builtin_write_OSCCONL(OSCCON | 0x40);

  OC2CON1 = 0x1C06;
  OC2CON2 = 0x001F;
  OC2RS = (uint16_t)(FCY / 1e3 - 1.);
  OC2R = OC2RS >> 2;
  OC2TMR = 0;
*/

}

void readspeed(void){
  LED1 = ON;
  case READ_A0:
      temp.w = read_analog(A0_AN);
      BD[EP0IN].address[0] = temp.b[0];
      BD[EP0IN].address[1] = temp.b[1];
      BD[EP0IN].bytecount = 2;
      BD[EP0IN].status = UOWN | DTS | DTSEN;
      break;
  case READ_A1:
      temp.w = read_analog(A1_AN);
      BD[EP0IN].address[0] = temp.b[0];
      BD[EP0IN].address[1] = temp.b[1];
      BD[EP0IN].bytecount = 2;
      BD[EP0IN].status = UOWN | DTS | DTSEN;
  break;
}



int16_t main(void) {
    init_elecanisms();
    USB_setup_vendor_callback = vendor_requests;
    init_usb();

    while (USB_USWSTAT != CONFIG_STATE) {
        #ifndef USB_INTERRUPT
          usb_service();
        #endif
    }
    while (1) {
        #ifndef USB_INTERRUPT
          usb_service();
          void movemotor(); 
        #endif
    }
}
