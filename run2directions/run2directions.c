// /*
// ** Copyright (c) 2018, Bradley A. Minch
// ** All rights reserved.
// **
// ** Redistribution and use in source and binary forms, with or without
// ** modification, are permitted provided that the following conditions are met:
// **
// **     1. Redistributions of source code must retain the above copyright
// **        notice, this list of conditions and the following disclaimer.
// **     2. Redistributions in binary form must reproduce the above copyright
// **        notice, this list of conditions and the following disclaimer in the
// **        documentation and/or other materials provided with the distribution.
// **
// ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// ** POSSIBILITY OF SUCH DAMAGE.
// */

#include "elecanisms.h"
#include "usb.h"
#include <stdio.h>

#define SET_MODE        100
#define SET_DUTY_VAL    102
#define ENC_READ_REG    103
#define GET_SMOOTH      104

#define ENC_MISO            D1
#define ENC_MOSI            D0
#define ENC_SCK             D2
#define ENC_CSn             D3
#define ENC_MISO_DIR        D1_DIR
#define ENC_MOSI_DIR        D0_DIR
#define ENC_SCK_DIR         D2_DIR
#define ENC_CSn_DIR         D3_DIR
#define ENC_MISO_RP         D1_RP
#define ENC_MOSI_RP         D0_RP
#define ENC_SCK_RP          D2_RP

uint16_t even_parity(uint16_t v) {
    v ^= v >> 8;
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return v & 1; }

WORD enc_readReg(WORD address) {
    WORD cmd, result;
    uint16_t temp;

    cmd.w = 0x4000 | address.w;         // set 2nd MSB to 1 for a read
    cmd.w |= even_parity(cmd.w) << 15;

    ENC_CSn = 0;
    SPI2BUF = (uint16_t)cmd.b[1];
    while (SPI2STATbits.SPIRBF == 0) {}
    temp = SPI2BUF;
    SPI2BUF = (uint16_t)cmd.b[0];
    while (SPI2STATbits.SPIRBF == 0) {}
    temp = SPI2BUF;
    ENC_CSn = 1;
    __asm__("nop");     // p.12 of the AS5048 datasheet specifies a minimum
    __asm__("nop");     //   high time of CSn between transmission of 350ns
    __asm__("nop");     //   which is 5.6 Tcy, so do nothing for 6 Tcy.
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    ENC_CSn = 0;
    SPI2BUF = 0;
    while (SPI2STATbits.SPIRBF == 0) {}
    result.b[1] = (uint8_t)SPI2BUF;
    SPI2BUF = 0;
    while (SPI2STATbits.SPIRBF == 0) {}
    result.b[0] = (uint8_t)SPI2BUF;
    ENC_CSn = 1;
    return result;
}



// void goto_zero(void){
//     all_off(); }

// void read_anglevalue(void){
//     all_off();
    // uint16_t j;
    // WORD temp;
    //
    // if (USB_setup.bRequest = 104) {
    //     j = USB_setup.wValue.w;
    //     // if(j < 180) { }
    //     // if(j > 180) {}
    //     BD[EP0IN].bytecount = 0;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN; }
    //
    // if (USB_setup.bRequest = 103) {
    //     temp = enc_readReg(USB_setup.wValue);
    //     BD[EP0IN].address[0] = temp.b[0];  //EP0IN set to 1 in buffer descriptor table
    //     BD[EP0IN].address[1] = temp.b[1];
    //     BD[EP0IN].bytecount = 2;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN;    // toegther the ORs are 11001000      // send packet as DATA1, set UOWN bit
    // }
// }

// void maintain_position(void){
//     all_off();
    // BD[EP0IN].bytecount = 0;
    // BD[EP0IN].status = UOWN | DTS | DTSEN;
    //
    // if (read_angle() > 180) {
    //     BD[EP0IN].bytecount = 0;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN;
    //     // int c = 0;
    //     // if(c == 0){
    //     //     all_off();
    //     //     c = 1; }
    //     // if(c == 1) {go_left_nostop(); }
    // }
    // if (read_angle() < 180 && read_angle() > 0) {
    //     BD[EP0IN].bytecount = 0;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN;
    //     int c = 0;
    //     if(c == 0){
    //         all_off();
    //         c = 1; }
    //     if(c == 1){ go_right_nostop(); }
    // }
// }

// int read_angle(void){
//     all_off();
//     return 0;
    // uint16_t j;
    // if (USB_setup.bRequest = 104) {
    //     j = USB_setup.wValue.w;
    //     BD[EP0IN].bytecount = 0;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN;
    //     return j;
    // }
// }
//
// void validate_angle(int a){
//     uint16_t b = get_smooth_function();
// }

// int get_smooth_function(void){
//     WORD temp;
//     uint16_t a;
//     switch (USB_setup.bRequest) {
//         case SET_MODE:
//             BD[EP0IN].bytecount = 0;
//             BD[EP0IN].status = UOWN | DTS | DTSEN;

//             break;
//         case ENC_READ_REG: //103

//             temp = enc_readReg(USB_setup.wValue);
//             BD[EP0IN].address[0] = temp.b[0];
//             BD[EP0IN].address[1] = temp.b[1];
//             BD[EP0IN].bytecount = 2;
//             BD[EP0IN].status = UOWN | DTS | DTSEN;
//             break;
//         case GET_SMOOTH: //104
//             a = USB_setup.wValue.w;
//             BD[EP0IN].bytecount = 0;
//             BD[EP0IN].status = UOWN | DTS | DTSEN;
//             break;
//         default:
//             USB_error_flags |= REQUEST_ERROR;
//         }
//     }


    // if (USB_setup.bRequest == 104) {
    //     uint16_t n;
    //     // uint16_t n = USB_setup.wValue.w;
    //     n = USB_setup.wValue.w;
    //     BD[EP0IN].bytecount = 0;
    //     BD[EP0IN].status = UOWN | DTS | DTSEN;
    //     return n; }

// void constant_angle_top(int start_angle){
//     if(start_angle > 180){
//      }
// }







int max_val = 7999;
int i1 = 0;
int i2 = 0;
int chkang;
volatile uint16_t angle;
uint8_t movetozero = 0;

void all_off(void){
    OC1R = 0;
    OC2R = 0; }

void go_left(void){
    all_off();
    OC1R = 1999;  }          // turn this (OC1R) to zero to turn off the motor

void go_right(void){
    all_off();
    OC2R = 1999; }             // turn this (OC2R) to zero to turn off the motor

void go_left_nostop(void){ OC1R = 1999;}
void go_right_nostop(void){ OC2R = 1999;}

int check_angle(angle){
    if (angle < 80)  { chkang = 1; }
    if (angle > 100) { chkang = 2; }

    return chkang;
}

void move_to_zero(){
    LED1 = !LED1;
    

}



void vendor_requests(void) {
    WORD temp;
    uint16_t j;// angle;

    if (movetozero) {
        move_to_zero();
    }
    switch (USB_setup.bRequest) {

        case SET_MODE:  // 100
            j = USB_setup.wValue.w;
            if(j == 0) {all_off(); }
            if(j == 1) {go_left(); }
            if(j == 2) {go_right(); }
            if(j == 3) {movetozero = 1; } else { movetozero = 0; }
            // if(j == 3) {move_to_zero(angle); }
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case ENC_READ_REG:  // 103
            temp = enc_readReg(USB_setup.wValue);
            BD[EP0IN].address[0] = temp.b[0];
            BD[EP0IN].address[1] = temp.b[1];
            BD[EP0IN].bytecount = 2;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        case GET_SMOOTH:  // 104
            angle = USB_setup.wValue.w;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;

        default:
            USB_error_flags |= REQUEST_ERROR;
    }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

int16_t main(void) {

    init_elecanisms();
    uint8_t *RPOR, *RPINR;
    uint8_t *RPORa, *RPINRa;

    D7_DIR = OUT;
    D7 = 0;
    D8_DIR = OUT;
    D8 = 0;

    RPOR = (uint8_t *)&RPOR0;
    RPINR = (uint8_t *)&RPINR0;
    RPORa = (uint8_t *)&RPOR1;
    RPINRa = (uint8_t *)&RPINR1;

    __builtin_write_OSCCONL(OSCCON & 0xBF);
        RPOR[D8_RP] = OC2_RP;
        RPOR[D7_RP] = OC1_RP;
    __builtin_write_OSCCONL(OSCCON | 0x40);

    OC1CON1 = 0x1C06;
    OC1CON2 = 0x001F;
    OC1RS = (uint16_t)(FCY / 1e3 - 1.);
    OC1TMR = 0;

    OC2CON1 = 0x1C06;
    OC2CON2 = 0x001F;
    OC2RS = (uint16_t)(FCY / 1e3 - 1.);
    OC2TMR = 0;

    ENC_CSn_DIR = OUT; ENC_CSn = 1;
    ENC_SCK_DIR = OUT; ENC_SCK = 0;
    ENC_MOSI_DIR = OUT; ENC_MOSI = 0;
    ENC_MISO_DIR = IN;

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPINR[MISO2_RP] = ENC_MISO_RP;
    RPOR[ENC_MOSI_RP] = MOSI2_RP;
    RPOR[ENC_SCK_RP] = SCK2OUT_RP;
    __builtin_write_OSCCONL(OSCCON | 0x40);

    SPI2CON1 = 0x003B;              // SPI2 mode = 1, SCK freq = 8 MHz
    SPI2CON2 = 0;
    SPI2STAT = 0x8000;

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
        #endif
    }

}
