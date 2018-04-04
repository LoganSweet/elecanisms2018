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
#

import Tkinter as tk
import time
import csv
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

##############################################################################
##############################################################################
##############################################################################


class miniproject2gui:

    def __init__(self):
        #self.dev = miniproject1.miniproject1()
        self.dev = mp2()
        if self.dev.dev >= 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('Miniproject2 CSV GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            fm.pack(side = tk.TOP)
            # dutyslider = tk.Scale(self.root, from_ = 0, to = 100, orient = tk.HORIZONTAL, command = self.set_duty_callback)
            # dutyslider.set(0)
            # dutyslider.pack(side = tk.TOP)
            #
            # b0 = tk.Button(fm, text = 'Mode 0: All Off', command = self.set_mode_callback_m0)
            # b0.pack(side = tk.TOP)
            # b1 = tk.Button(fm, text = 'Mode 1: Left', command = self.set_mode_callback_m1)
            # b1.pack(side = tk.TOP)
            # b2 = tk.Button(fm, text = 'Mode 2: Right', command = self.set_mode_callback_m2)
            # b2.pack(side = tk.TOP)
            # b3 = tk.Button(fm, text = 'Mode 3: Return to Zero Position', command = self.set_mode_callback_m3)
            # b3.pack(side = tk.TOP)
            # b4 = tk.Button(fm, text = 'Mode 4: Spring', command = self.set_mode_callback_m4)
            # b4.pack(side = tk.TOP)
            # b5 = tk.Button(fm, text = 'Mode 5: Damper', command = self.set_mode_callback_m5)
            # b5.pack(side = tk.TOP)
            # b6 = tk.Button(fm, text = 'Mode 6: Texture', command = self.set_mode_callback_m6)
            # b6.pack(side = tk.TOP)
            # b7 = tk.Button(fm, text = 'Mode 7: Wall', command = self.set_mode_callback_m7)
            # b7.pack(side = tk.TOP)

            self.enc_status = tk.Label(self.root, text = 'Full angle value is (angle) ?????')
            self.enc_status.pack(side = tk.TOP)

            self.enc_status_bare = tk.Label(self.root, text = 'Bitshift 6 angle is (angle255) ?????')
            self.enc_status_bare.pack(side = tk.TOP)

            self.fp_status = tk.Label(self.root, text = 'data you are sending is ????')
            self.fp_status.pack(side = tk.TOP)

            self.update_status()

    # def set_mode_callback_m0(self):
    #     self.dev.set_mode(0)
    #
    # def set_mode_callback_m1(self):
    #     self.dev.set_mode(1)
    #
    # def set_mode_callback_m2(self):
    #     self.dev.set_mode(2)

    def set_mode_callback_m3(self):
        self.dev.set_mode(3)
        self.dev.set_smooth()

    # def set_mode_callback_m4(self):
    #     self.dev.set_mode(4)
    #     self.dev.set_smooth()
    #
    # def set_mode_callback_m5(self):
    #     self.dev.set_mode(5)
    #     self.dev.set_smooth()
    #
    # def set_mode_callback_m6(self):
    #     self.dev.set_mode(6)
    #     self.dev.set_smooth()
    #
    # def set_mode_callback_m7(self):
    #     self.dev.set_mode(7)
    #     self.dev.set_smooth()

    def update_status(self):
        # self.enc_status.configure(text = 'Angle is {:04d}'.format(self.dev.get_angle() ))
        self.enc_status.configure(text = 'Full angle value is (angle) {:}'.format(self.dev.get_angle() ))
        self.enc_status_bare.configure(text = 'Bitshift 6 angle is (angle255) {:}'.format(self.dev.get_angle_shift() ))
        self.fp_status.configure(text = 'data you are sending is {:}'.format(self.dev.send_to_python() ))
        self.update_job = self.root.after(50, self.update_status)

        self.dev.set_smooth()  # this allows the C file to be updatee enough



        dataforcsv = [(self.dev.get_angle()* 360 / 16380), time.time()]
        with open(r'angledata_final4.csv', 'a') as f:
            writer = csv.writer(f)
            writer.writerow(dataforcsv)

        self.update_job = self.root.after(1, self.update_status)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()

if __name__=='__main__':
    gui = miniproject2gui()
    gui.root.mainloop()
