# python goes here

import usb.core
class usbtest:

    def __init__(self):
        self.READ_A0 = 0
        self.READ_A1 = 1
        self.SET_DUTY_VAL = 2
        self.GET_DUTY_VAL = 3
        self.GET_DUTY_MAX = 4
