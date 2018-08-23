# LX Lcd Vario Library
This is a arduino library to communicate with PCF8576 display controller in the LX Lcd Vario Display.

## Story
I fly in a gliding club in Nordhorn. Some time ago we installed an OpenVario in our DuoDiscus. However, there is no second display for the digital vario. Therefore the idea came up to address the old LX Vario with an Arduino to use it as a second display.
First trys to analyze the RS485 Bus failed. Then I think of solder only the display out an dirve it by myself. But then I figured out that there are two **PCF8576T** chips on the circuit board. A quick google search brings up some arduino projects, so i tried to control it.

## Hardware
### Display
The display is a segment lcd display. It has **154** segemnts. And every segement can be controlled individually.
IMAGES
### Circuit Board
There are __two__ circuit boards in the housing of the device.
The **top** one is mainly for the power distribution.
IMAGES
The **bottom** one is soldered directly to the display and has a few more chips:
* Two PCF8576T display driver
* Temic s-80C32-16 (I think its the microcontroller)
* AM27C010-90JC (EPROM)
* HC573A (Shift Register, don't know exactly for what)
IMAGES
The interesting board is the bottom one with the PCF8576T controller. The datasheet is available and shows that these chips can adressed via the I2C Protocol.
### PCF8576T
The PCF8576T are display controller for various lcd displays. They are used in many displays. The datasheet is very datailed. The most important things and settings you have to figure out are:
* The slave address of the PCF's

Two slave addresses are reserved for the PCF8576 controller. **B111000** and **B111001**. The slave address is set with the level on pin **SA0**. In the case of my display, both are set to GND, therefore the slave address of the devices are __B111000__.
* The device select of the PCF's
* The modeset of your display
* The blink mode of your display
