# Arduino-Processing-screen-color-capture

Program that records the screen in real time to create both 24 bit an 8 bit depth color channels, it's used with an Arduino connected
to PC by a usb cable, to control an RGB led strip according to the average color of the screen's pixels, with some variables for color
correction that can be setted before compiling or running the program.

The PC program is written using "Processing", the Arduino program is written with the Arduino IDE, pins 5,9,11 are respectively for
R,G,B cannels and create a PWM signal with a resolution of 8 bit per channel (0-255 for each color)

the next step will be to rewrite the code in pure Java and possibly in C to improve the performance
