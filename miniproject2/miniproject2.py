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
class mp2:

    def __init__(self):

        self.SET_MODE = 100
        self.SET_DUTY_VAL = 102
        self.ENC_READ_REG = 103
        self.GET_SMOOTH = 104
        self.GET_ANGLE_DATA = 105
        self.SEND_TO_PYTHON = 106

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
            raise ValueError('USB device not found')
        self.dev.set_configuration()

    def close(self):
        self.dev = None

    def get_angle(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.ENC_READ_REG, 0x3FFF, 0, 2)
        except usb.core.USBError:
            print "Could not send ENC_READ_REG vendor request for get_angle."
        else:
            # return int((((int(ret[0]) + 256 * int(ret[1])) & 0x3FFF ) >> 6) * 0.70588)
            return int( (int(ret[0]) + 256 * int(ret[1])) & 0x3FFF )

    def set_mode(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_MODE, val)
        except usb.core.USBError:
            print "Could not send SET_MODE val vendor request xxxx."

    def set_smooth_val(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.GET_SMOOTH, val)
        except usb.core.USBError:
            print "Could not send set_smooth_val vendor request."

    def set_smooth(self):
        val = self.get_angle()
        self.set_smooth_val(val)


    def get_angle_shift(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.ENC_READ_REG, 0x3FFF, 0, 2)
        except usb.core.USBError:
            print "Could not send ENC_READ_REG vendor request for get_angle."
        else:
            return int( ( (int(ret[0]) + 256 * int(ret[1]) ) & 0x3FFF ) >> 6)

    def send_to_python(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.SEND_TO_PYTHON, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send send_tom_python vendor request."
        else:
                return int(ret[0]) + 256 * int(ret[1])




    # def get_smooth_angle(self, val):
    #     try:
    #         # self.dev.ctrl_transfer(0x40, self.GET_SMOOTH, val)
    #         self.dev.ctrl_transfer(0x40, self.GET_SMOOTH, val)
    #     except usb.core.USBError:
    #         print "Could not send the get_smooth_angle things from python."

    # def get_smooth_angle(self, val):
    #     try:
    #         print "getting smooth"
    #         print val
    #         ret = self.dev.ctrl_transfer(0x40, self.GET_SMOOTH, val)
    #         print "got through smooth wothout errors"
    #     except usb.core.USBError:
    #         print "smooth error place"
    #         print val
    #         print "you done goofed in your thing Logan"
