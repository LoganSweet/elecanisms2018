import Tkinter as tk
import time
import miniproject1

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

        #with open('filename.csv', 'w') as f:
        #    f.write( (time.time()) + ',' + self.dev.get_angle() + '\n')
        print time.time()
        print self.dev.get_angle()

        self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()

if __name__=='__main__':
    gui = miniproject1gui()
    gui.root.mainloop()
