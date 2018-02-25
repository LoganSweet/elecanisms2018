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
    return v & 1;
}

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

void all_off(void){
    OC1R = 0;
    OC2R = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
}

void go_oneway(void){
    all_off();
    LED1 = 1;
    OC1R = 3999;               // turn this (OC1R) to zero to turn off the motor
}

void go_otherway(void){
    all_off();
    LED3 = 1;
    OC2R = 3999;               // turn this (OC1R) to zero to turn off the motor
}

void vendor_requests(void) {
    WORD temp;
    uint16_t i;
    uint16_t j;

    switch (USB_setup.bRequest) {
        case SET_MODE:
            j = USB_setup.wValue.w;
            if(j == 0) {all_off(); }
            if(j == 1) { go_oneway(); }
            if(j == 2) { go_otherway(); }

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
    RPOR[D7_RP] = OC1_RP;                          // pin number
    __builtin_write_OSCCONL(OSCCON | 0x40);

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPOR[D8_RP] = OC2_RP;                          // pin number
    __builtin_write_OSCCONL(OSCCON | 0x40);

    OC1CON1 = 0x1C06;
    OC1CON2 = 0x001F;
    OC1RS = (uint16_t)(FCY / 1e3 - 1.);
    // OC1R = OC1RS >> 2;
    OC1TMR = 0;

    OC2CON1 = 0x1C06;
    OC2CON2 = 0x001F;
    OC2RS = (uint16_t)(FCY / 1e3 - 1.);
    // OC1R = OC1RS >> 2;
    OC2TMR = 0;




    ////// for pin 7
    // Configure encoder pins and connect them to SPI2
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




    //
    // D8_DIR = OUT;                                   // pin number here
    // D8 = 0;                                         // pin number
    // D7_DIR = OUT;
    // D7 = 0;
    //
    // RPOR = (uint8_t *)&RPOR0;
    // RPINR = (uint8_t *)&RPINR0;
    //
    // __builtin_write_OSCCONL(OSCCON & 0xBF);
    // RPOR[D8_RP] = OC1_RP;                          // pin number
    // __builtin_write_OSCCONL(OSCCON | 0x40);
    //
    // OC1CON1 = 0x1C06;
    // OC1CON2 = 0x001F;
    // OC1RS = (uint16_t)(FCY / 1e3 - 1.);
    // // OC1R = OC1RS >> 2;
    // OC1R = 0;               // turn this (OC1R) to zero to turn off the motor
    // OC1TMR = 0;



///////////////////////


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

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
