# Elecanisms Miniproject 0: Blinking LED and Coin Collector
**Maggie Jakus and Logan Sweet**
Due: Tuesday, January 30

### Introduction ###
The purpose of this lab was to familiarize ourselves with the tools we need to program the PIC24 microcontroller. In addition to installing the needed tools, we also created a simple C program to change the behavior of the onboard LEDs.


### Tools and Process ###
To make use of the PIC24 microcontroller, we need some digital tools to interface with the microcontroller and load our C programs. After downloading Scons and the MPLAB compiler, we loaded an example blink program onto the PIC24 by way of the following steps:

1. Update and save the C program in the same directory as the Sconstruct file.
2. Open a command window and navigate to that directory. Type Scons and hit enter. This will compile your code and create a hex file of the program instructions.
3. Navigate to Bootloader> Software and type python bootloader.py and hit enter. This will open a GUI window.
4. The GUI may say that it cannot connect to a USB bootloader device. To connect the microcontroller in bootloader mode, plug it into a USB port while holding down switch 1, then click connect. Once you have connected, you can release button 1.
5. Click File> Import Hex and navigate to the directory where you just ran Scons. Choose the hex file you created with Scons.
6. Click write to write the program to your microcontroller.
7. Click disconnect/run to exit bootloader mode, then your microcontroller will run your C code.

### Hardware ###
We used a button to provide a signal to the digital pin on the microcontroller. Our button had one side connected to the onboard Vdd pin, and the other side connected to a 10k Ohm pulldown resistor to ground. We connected digital pin 0 to the button on the second button pin, between the button and the pulldown resistor. This means that when the button is not pressed D0 sees ground, and when it is pressed D0 seed Vdd.


### C Code ###
We started with the example code provided to the class, and edited it so that LED 1 was activated by the signal from digital pin 0. Although we are familiar with the operation of both the blinkpoll and blinkint programs, blinkpoll was easier to understand since it did not use the interrupt handler. Since we more fully understood it, we chose to use blinkpoll as our starting point.

We implemented a very basic change with the button, and essentially replaced SW2 with the breadboarded button so that it would control LED1. To do this, we initially replaced "SW2" in the program with "D0". Since the button was connected to the D0 pin as explained above, pressing that button changed the behavior of the LED. However,  we found that this did the opposite of the switch, and turned off the LED when the button was pressed. We switched the order of the arguments in the ternary conditional to fix this, so that the button would light up when the button was pressed.
Our code for the simple button functionality is below.

```C

#include "elecanisms.h"

int16_t main(void) {
    init_elecanisms();

    T1CON = 0x0030;         // set Timer1 period to 0.5s
    PR1 = 0x7A11;

    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1

    LED2 = ON;

    while (1) {
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
            LED2 = !LED2;           // toggle LED2
        }
        LED1 = (D0 == 0) ? OFF : ON;   // turn LED1 on if breadboard button is pressed
        LED3 = (SW3 == 0) ? ON : OFF;   // turn LED3 on if SW3 is pressed
    }
}

```


### Conclusion ###
We spent quite a bit of time during this miniproject trying to get the coin acceptor to work, and were a little frustrated when we were not able to make that happen, but we are looking forward to solving the issue (with more help from instructors) in a future miniproject.
Since neither of us have any experience with microcontrollers other than Arduino, we are missing a bit of understanding around exactly what is happening during the process before we upload out code. Even though it doesn't (yet) affect our ability to write and upload firmware, we would like to have a better understanding of what is going on behind the scenes.


### Logan's Extra Reflection: ###
##### A Story of Mostly Failure and Maybe Some Learning #####

Since my new and improved 14-credit semester gives me actual free time, I ended up dumping quite a few hours over the weekend and on Monday night into trying to make our code do something more interesting than light a button. I tried a lot of things and went down a lot of paths, but I never got anything to work at all. These things are summarized below, and some of the code I tried to work on is in the blinknew folder in the main elecanisms2018 directory on my github.

My first goal was to manually debounce the button with a timer. To make sure I was able to use time.h, I tried to create simple functionality that would change which LED was lit based on how much time has elapsed. I used a blink function from Brenna's elecanisms github as a refernece at first, but there were too many pieces I didn't understand. I tried using if statements that evaluated the difference between recorded time at the beginning of the program and the current time to light two different LEDs, but only ever saw the second LED light up, despite there being plenty of time to observe the first one.

I then looked at some finite state machine code that Brad showed during eclectroncs last semester. I was able to follow what was happening in each line, but there seemed to be some built-in functions that I wasn't able to create functional versions of myself.

Finally, I spent a lot of time reading the family reference guide and the datasheet for the microcontroller, and tried to implement a simple switch from one LED to another using timer 2. I had a lot of help from another student interpreting the datasheets and, definitely feel like I learned a lot as part of the process.  I have a better understanding for how the timer period and prescaler work with the clock to essentially create a slower clock, and understand generally that this information is stored in the register associated with a timer.

However, I still am missing how to determine what the correct name of the register is. The control register diagrams never list which bits are PR2, for example, so I do not know what I am controlling with PR2. I read all of the .h files in the lib folder from elecanisms2018, but I was not able to find a place where that name was assigned to asnything else. The role of those files, especially elecanisms.h, is still unclear to me and I hope that understanding them more will be useful moving forward.

Ultimately this felt like a very frustrating project since I spent hours reading and trying code with nothing to show for it, but I think I did get some good experience reading a new type of datasheet and I hope to get more help from instructors earlier on so that I can be more successful next time.

### References ###
We used this site as a resource for formatting markdown:
- https://guides.github.com/features/mastering-markdown/

We used the following websites as resources for C:
- https://www.geeksforgeeks.org/operators-c-c/
- https://www.tutorialspoint.com/c_standard_library/index.htm
- https://en.wikipedia.org/wiki/C_date_and_time_functions


Example code and guidance for the miniproject was provided by instructors, the ninja, and some advice from other students in the course.
