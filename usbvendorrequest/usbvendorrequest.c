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

// #define READ_A0             100
// #define READ_A1             101
// #define SET_DUTY_VAL        102
// #define GET_DUTY_VAL        103
// #define GET_DUTY_MAX        104
// #define ENC_READ_REG        105

#define TOGGLE_M1         110
#define TOGGLE_M2         113
#define TOGGLE_M3         112
// #define TOGGLE_M4         113
#define READ_M1            114
#define READ_M2            115
#define READ_M3            116
// #define READ_M4            117

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

void vendor_requests(void) {
    WORD temp;
    uint16_t i;

    switch (USB_setup.bRequest) {
        // case READ_MODE:
        //     BD[EP0IN].address[0] = SW1 ? 1 : 0;
        //     BD[EP0IN].bytecount = 1;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        case TOGGLE_M1:
            // LED2 = 0;
            // LED1 = 1;
            // LED3 = 0;
            LED1 = 1;
            LED2 = 0;
            LED3 = 0;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case TOGGLE_M2:
            // LED2 = 1;
            // LED1 = 0;
            // LED3 = 0;
            LED2 = 1;
            LED1 = 1;
            LED3 = 1;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case TOGGLE_M3:
            // LED2 = 0;
            // LED1 = 0;
            // LED3 = 1;
            LED1 = 0;
            LED2 = 0;
            LED3 = 1;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        // case TOGGLE_M4:
        //     LED1 = 1;
        //     LED2 = 1;
        //     LED3 = 1;
        //     D0 = 1;
        //     BD[EP0IN].bytecount = 0;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        case READ_M1:
            BD[EP0IN].address[0] = LED1 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case READ_M2:
            BD[EP0IN].address[0] = LED2 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case READ_M3:
            BD[EP0IN].address[0] = LED3 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        // case READ_M4:
        //     BD[EP0IN].address[0] = (LED3 & LED2 & LED1) ? 1 : 0;
        //     BD[EP0IN].bytecount = 1;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case READ_A0:
        //     temp.i = read_analog(A0_AN);
        //     BD[EP0IN].address[0] = temp.b[0];
        //     BD[EP0IN].address[1] = temp.b[1];
        //     BD[EP0IN].bytecount = 2;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case READ_A1:
        //     temp.w = read_analog(A1_AN);
        //     BD[EP0IN].address[0] = temp.b[0];
        //     BD[EP0IN].address[1] = temp.b[1];
        //     BD[EP0IN].bytecount = 2;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case SET_DUTY_VAL:
        //     OC1R = USB_setup.wValue.w;
        //     BD[EP0IN].bytecount = 0;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case GET_DUTY_VAL:
        //     temp.w = OC1R;
        //     BD[EP0IN].address[0] = temp.b[0];
        //     BD[EP0IN].address[1] = temp.b[1];
        //     BD[EP0IN].bytecount = 2;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case GET_DUTY_MAX:
        //     temp.w = OC1RS;
        //     BD[EP0IN].address[0] = temp.b[0];
        //     BD[EP0IN].address[1] = temp.b[1];
        //     BD[EP0IN].bytecount = 2;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        // case ENC_READ_REG:
        //     temp = enc_readReg(USB_setup.wValue);
        //     BD[EP0IN].address[0] = temp.b[0];
        //     BD[EP0IN].address[1] = temp.b[1];
        //     BD[EP0IN].bytecount = 2;
        //     BD[EP0IN].status = UOWN | DTS | DTSEN;
        //     break;
        default:
            USB_error_flags |= REQUEST_ERROR;
    }
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

int16_t main(void) {
    uint8_t *RPOR, *RPINR;
    init_elecanisms();
//
//     D8_DIR = OUT;                                   // pin number here
//     D8 = 0;                                         // pin number
//     RPOR = (uint8_t *)&RPOR0;
//     RPINR = (uint8_t *)&RPINR0;
//
//     __builtin_write_OSCCONL(OSCCON & 0xBF);
//     RPOR[D8_RP] = OC1_RP;                          // pin number
//     __builtin_write_OSCCONL(OSCCON | 0x40);
//
//     OC1CON1 = 0x1C06;
//     OC1CON2 = 0x001F;
//
//     OC1RS = (uint16_t)(FCY / 1e3 - 1.);
//     OC1R = OC1RS >> 2;
//     OC1TMR = 0;

///////////////////////////////////////////////////////
//////////////// from encoder test ////////////////////
///////////////////////////////////////////////////////

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
