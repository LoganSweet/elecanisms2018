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

import Tkinter as tk
import time
import run2directions

class run2directionsgui:

    def __init__(self):
        self.dev = run2directions.runtwo()
        if self.dev.dev >= 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('Run Two Directions GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            fm.pack(side = tk.TOP)

            b0 = tk.Button(fm, text = 'Mode 0: All Off', command = self.set_mode_callback_m0)
            b0.pack(side = tk.TOP)
            b1 = tk.Button(fm, text = 'Mode 1: Left', command = self.set_mode_callback_m1)
            b1.pack(side = tk.TOP)
            b2 = tk.Button(fm, text = 'Mode 2: Right', command = self.set_mode_callback_m2)
            b2.pack(side = tk.TOP)
            b3 = tk.Button(fm, text = 'Mode 3: Maintain Position', command = self.set_mode_callback_m3)
            b3.pack(side = tk.TOP)

            self.enc_status = tk.Label(self.root, text = 'Anlge is ?????')
            self.enc_status.pack(side = tk.TOP)

            self.update_status()

    def set_mode_callback_m0(self):
        self.dev.set_mode(0)

    def set_mode_callback_m1(self):
        self.dev.set_mode(1)

    def set_mode_callback_m2(self):
        self.dev.set_mode(2)

    def set_mode_callback_m3(self):
        self.dev.set_mode(3)
        # a = self.dev.get_angle()
        # self.dev.get_smooth_angle(a)
        self.dev.set_smooth()

    def update_status(self):
        # self.enc_status.configure(text = 'Angle is {:04d}'.format(self.dev.get_angle() ))
        self.enc_status.configure(text = 'Angle is {:}'.format(self.dev.get_angle() ))
        self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()

if __name__=='__main__':
    gui = run2directionsgui()
    gui.root.mainloop()
