# Elecanisms Miniproject 0: Blinking LED and Coin Collector
**Maggie Jakus and Logan Sweet**
Due: Tuesday, January 30

### Introduction ###
The purpose of this lab was to familiarize ourselves with the tools we need to program the PIC24 microcontroller. In addition to installing the needed tools, we also created a simple C program to change the behavior of the onboard LEDs. 


### Tools and Process ###
To make use of the PIC24 microcontroller, we need some digital tools to interface with the microcontroller and load our C programs. After downloading Scons and the MPLAB compiler, we loaded an example blink program onto the PIC24 by way of the following steps: 

1. Update and save the C program in the same directory as the Sconstruct file. 
2. Open a command window and navigate to that directory. Type Scons and hit enter. 
3. Navigate to Bootloader> Software and type python bootloader.py and hit enter. This will open a GUI window. 
4. The GUI may say that it cannot connect to a USB bootloader device. To connect the microcontroller in bootloader mode, plug it into a USB port while holding down switch 1, then click connect. Once you have connected, you can release button 1. 
5. Click File> Import Hex and choose the C file you wrote. 
6. Click write to write the program to your microcontroller. 
7. Click disconnect/run to exit bootloader mode, then your microcontroller will run your C code. 

### Hardware ###
We used a button to provide a signal to the digital pin on the microcontroller. Our button had one side connected to the onboard Vdd pin, and the other side connected to a 10k Ohm pulldown resistor to ground. We connected digital pin 0 to the button on the second button pin, between the button and the pulldown resistor. This means that when the button is not pressed D0 sees ground, and when it is pressed D0 seed Vdd. 


### C Code ###
We started with the example code provided to the class, and edited it so that LED 1 was activated by the signal from digital pin 0. Although we are familiar with the operation of both the blinkpoll and blinkint programs, blinkpoll was easier to understand since it did not use the interrupt handler. Since we more fully understood it, we chose to use blinkpoll as our starting point. 

We first implemented a very basic change with the button, and essentially replaced SW2 with the breadboarded button so that it would control LED1. To do this, we simply replaced "SW2" in the program with "D0". Since the button was connected to the D0 pin as explained above, pressing that button changed the behavior of the LED. 

Our code for the simple button functionality is below. 

```C


Copy and paste the code here and it will be in the correct format!! 


```

### Conclusion ###
We spent quite a bit of time during this miniproject trying to get the coin acceptor to work, and were a little frustrated when we were not able to make that happen, but we are looking forward to solving the issue (with more help from instructors) in a future miniproject. 
Since neither of us have any experience with microcontrollers other than Arduino, we are missing a bit of understanding around exactly what is happening during the process before we upload out code. Even though it doesn't (yet) affect our ability to write and upload firmware, we would like to have a better understanding of what is going on behind the scenes. 


### References ###
We used this site as a resource for formatting markdown: https://guides.github.com/features/mastering-markdown/ 

Example code and guidance for the miniproject was provided by instructors, the ninja, and some advice from other students in the course. 


