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
import miniproject1
import csv

class miniproject1gui:

    def __init__(self):
        self.dev = miniproject1.miniproject1()
        if self.dev.dev >= 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('Miniproject1 GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            fm.pack(side = tk.TOP)
            #dutyslider = tk.Scale(self.root, from_ = 0, to = 100, orient = tk.HORIZONTAL, showvalue = tk.FALSE, command = self.set_duty_callback)
            dutyslider = tk.Scale(self.root, from_ = 0, to = 100, orient = tk.HORIZONTAL, command = self.set_duty_callback)
            dutyslider.set(25)
            dutyslider.pack(side = tk.TOP)

            self.a0_status = tk.Label(self.root, text = 'A0 is currently ????')
            self.a0_status.pack(side = tk.TOP)
            self.a1_status = tk.Label(self.root, text = 'A1 is currently ????')
            self.a1_status.pack(side = tk.TOP)

            self.t0_status = tk.Label(self.root, text = 'T0 is ????')
            self.t0_status.pack(side = tk.TOP)
            self.t1_status = tk.Label(self.root, text = 'T1 is ????')
            self.t1_status.pack(side = tk.TOP)

            self.enc_status = tk.Label(self.root, text = 'Anlge is ?????')
            self.enc_status.pack(side = tk.TOP)

            self.update_status()

    def set_duty_callback(self, value):
        self.dev.set_duty(float(value))

    def update_status(self):
        self.a0_status.configure(text = 'A0 is currently {:}'.format(self.dev.read_a0() ))
        self.a1_status.configure(text = 'A1 is currently {:}'.format(self.dev.read_a1() ))
        self.t0_status.configure(text = 'T0 is currently {:}'.format(self.dev.read_a0() - 500 ))
        self.t1_status.configure(text = 'T1 is currently {:}'.format(self.dev.read_a1() - 500 ))

        self.enc_status.configure(text = 'Angle is {:1}'.format(self.dev.get_angle() * 360 / 16380 ))
        #self.enc_status.configure(text = 'Angle is {:1}'.format(self.dev.get_angle()  ))



        # print time.time()
        # print (self.dev.get_angle() * 360 / 16380 )

        self.update_job = self.root.after(50, self.update_status)

        # from paige:
        # with open(r'document.csv', 'a') as f:
        #      writer = csv.writer(f)
        #    writer.writerow(angleAndTime)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()

if __name__=='__main__':
    gui = miniproject1gui()
    gui.root.mainloop()
