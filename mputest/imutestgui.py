
import Tkinter as tk
#import encodertest
import mputest
import imutest
import usbtest

class imutestgui:

    def __init__(self):
        self.dev = imutest.imutest()
        if self.dev.dev >= 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('IMU Test GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            fm.pack(side = tk.TOP)
            self.sw1_status = tk.Label(self.root, text = 'aX is currently ?')
            self.sw1_status.pack(side = tk.TOP)
            self.sw2_status = tk.Label(self.root, text = 'aY is currently ?')
            self.sw2_status.pack(side = tk.TOP)
            self.sw3_status = tk.Label(self.root, text = 'aZ is currently ?')
            self.sw3_status.pack(side = tk.TOP)

            self.sw4_status = tk.Label(self.root, text = 'gX is currently ?')
            self.sw4_status.pack(side = tk.TOP)
            self.sw5_status = tk.Label(self.root, text = 'gY is currently ?')
            self.sw5_status.pack(side = tk.TOP)
            self.sw6_status = tk.Label(self.root, text = 'gZ is currently ?')
            self.sw6_status.pack(side = tk.TOP)

            self.update_status()

    def update_status(self):
        self.sw1_status.configure(text = 'aX is currently {:05d}'.format(accel[0])
        self.sw2_status.configure(text = 'aY is currently {:05d}'.format(accel[0])
        #self.sw2_status.configure(text = 'aY is currently {:05d}'.format(self.dev.read_sw2()))
        self.sw3_status.configure(text = 'aZ is currently {:05d}'.format(self.dev.read_sw3()))

        self.sw4_status.configure(text = 'gX is currently {:05d}'.format(self.dev.read_sw1()))
        self.sw5_status.configure(text = 'gY is currently {:05d}'.format(self.dev.read_sw2()))
        self.sw6_status.configure(text = 'gZ is currently {:05d}'.format(self.dev.read_sw3()))

        self.enc_status.configure(text = 'Angle is {:05d}'.format(self.dev.get_angle()))
        #self.enc_status.configure(text = 'Angle is {!s}'.format(self.dev.get_angle()))
        self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()

if __name__=='__main__':
    gui = encodertestgui()
    gui.root.mainloop()
