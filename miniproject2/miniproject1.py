# /*
# ** Copyright (c) 2018, Bradley A. Minch
# ** All rights reserved.
# **
# ** Redistribution and use in source and binary forms, with or without
# ** modification, are permitted provided that the following conditions are met:
# **
# **     1. Redistributions of source code must retain the above copyright
# **        notice, this list of conditions and the following disclaimer.
# **     2. Redistributions in binary form must reproduce the above copyright
# **        notice, this list of conditions and the following disclaimer in the
# **        documentation and/or other materials provided with the distribution.
# **
# ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# ** POSSIBILITY OF SUCH DAMAGE.
# */

import usb.core
class miniproject1:

    def __init__(self):
        self.READ_A0 = 100
        self.READ_A1 = 101
        self.SET_DUTY_VAL = 102
        self.GET_DUTY_VAL = 103
        self.GET_DUTY_MAX = 104
        self.ENC_READ_REG = 105

# AS5048A Register Map for reading from angle sensor
        self.ENC_NOP = 0x0000                       #0
        self.ENC_CLEAR_ERROR_FLAG = 0x0001          #1
        self.ENC_PROGRAMMING_CTRL = 0x0003          #3
        self.ENC_OTP_ZERO_POS_HI = 0x0016           #22
        self.ENC_OTP_ZERO_POS_LO = 0x0017           #23
        self.ENC_DIAG_AND_AUTO_GAIN_CTRL = 0x3FFD   #16381
        self.ENC_MAGNITUDE = 0x3FFE                 #16382
        self.ENC_ANGLE_AFTER_ZERO_POS_ADDER = 0x3FFF#16383

        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003) # looks for something in usb port

        if self.dev is None:            # self/dev is your pic
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()


    def close(self):
        self.dev = None

    def read_a0(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.READ_A0, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send READ_A0 vendor request."
        else:
            return int(ret[0]) + 256 * int(ret[1])

    def read_a1(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.READ_A1, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send READ_A1 vendor request."
        else:
            return int(ret[0]) + 256 * int(ret[1])

    def set_duty_val(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_DUTY_VAL, val)            #0x40 does something on microcontroller
        except usb.core.USBError:
            print "Could not send SET_DUTY_VAL vendor request."

    def get_duty_val(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_DUTY_VAL, 0, 0, 2)  #0xc0 returns something to python
        except usb.core.USBError:
            print "Could not send GET_DUTY_VAL vendor request."
        else:
            return int(ret[0]) + 256 * int(ret[1])

    def get_duty_max(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_DUTY_MAX, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send GET_DUTY_MAX vendor request."
        else:
            return int(ret[0]) + 256 * int(ret[1])

    def set_duty(self, duty_cycle):
        val = int(round(duty_cycle * self.get_duty_max() / 100.))
        self.set_duty_val(val)

    def get_duty(self):
        return 100. * self.get_duty_val() / self.get_duty_max()

############################################################
############################################################
############################################################

    def enc_readReg(self, address):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.ENC_READ_REG, address, 0, 2)
        except usb.core.USBError:
            print "Could not send ENC_READ_REG vendor request for enc_readReg."
        else:
            return ret

    def get_angle(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.ENC_READ_REG, 0x3FFF, 0, 2)
        except usb.core.USBError:
            print "Could not send ENC_READ_REG vendor request for get_angle."
        else:
            return (int(ret[0]) + 256 * int(ret[1])) & 0x3FFF
