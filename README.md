# Arduino-Processing-screen-color-capture

Program that records the screen in real time to create both 24 bit an 8 bit depth color channels, it's used with an Arduino connected
to PC with an USB cable, to control an RGB led strip according to the average color of the screen's pixels, with some variables for color correction that can be setted before compiling or running the program  (I should add the possibility of file configuration).

The PC program is written using "Processing", the Arduino program is written with the Arduino IDE, pins 5,9,11 are respectively for
R,G,B channels and create a PWM signal with a resolution of 8 bit per channel (0-255 for each color)

the next step will be to rewrite the code in pure Java and possibly in C to improve the performance and add an interface to change the USB port and the refresh rate as well as color correction's values while the program is compiled or is running

use the "_SA" version of the screenCap program when no arduino is connected or when configuring the COM port
( **this is due to my bad programming skills on Processing:** the _SA version has an 'on-screen' preview of the led strip's theorical color that can be edited using the constants in the source code, it also runs without an Arduino connected to the PC, while the normal version need an Arduino connected to avoid program crash)
to select the Arduino port to which send data, run the "_SA" program and refer tho the processing IDE's log window

## New (but old)
Added a spaghetti dish that use C++ and Windows API to do almost the same things but on a terminal, extremely simplified and uglier to see but... hey, it is C++, it's intended not to be beautiful, but to be fast and lightweight (this doesn't crash even without the USB ;) )
