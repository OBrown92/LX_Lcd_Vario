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
The interesting board is the bottom one with the PCF8576T controller. The datasheet is available and shows that these chips can adressed via the I2C Protocol. To control the PCF's i soldered two wires on the circuit board and removed two pins from the Temic microcontroller. Luckily there are two solder points for the SCL and SDA so it's very easy. It's also very easy to power the Display, just put 5V (for better contrast only 4V) on the Pins in the picture.
IMAGE
### PCF8576T
The PCF8576T are display controller for various lcd displays. They are used in many displays. The datasheet is very datailed. The most important things and settings you have to figure out are:
* The slave address of the PCF's
* The device select of the PCF's
* The modeset of your display
* The blink mode of your display

#### Slave address
Two slave addresses are reserved for the PCF8576 controller. **B111000** and **B111001**. The slave address is set with the level on pin **SA0**. In the case of my display, both are set to GND, therefore the slave address of the devices are _B111000_.
#### Device select
You can address up to 8 devices with one slave address. That means you can combine 16 PCF's on one I2C Bus. The device select is set via the **A0**, **A1** and **A2** pins. In the case of the LX Lcd Vario, the first chip has all pins connected to ground and the second one has **A0** connected to VDD.
### Modeset
You have to figure out the modeset of your display. The PCF can control displays with backplane drive configuration of static or 2/3/4 backplane multiplexing. You have to set the display bias configuration to static, 1/2 or 1/3. Also you can enable or disable the display or put it in a power saving mode. The LX Lcd Vario display has 4 Backplanes, a bias of 1/2 and no power saving.
### Blink mode
The blink mode can be set to 2 Hz, 1 Hz or 0,5 Hz and also to alternation blinking. For more details, see the datasheet. The LX Lcd Display don't blink. Someone wrote in a thread "never blink".
## Software
### Library
I first want to write a universal library for the PCF8576 but it turned out that the displays are very various designed. I will extract the main parts later and put them online.

The library is now optimized for the LX Lcd Display. It works with a **buffer** and a **show()** function. Every segment you want to show must be put in a buffer. Thats important because some segements share the same byte and have to combined via _bitwise or_ before write them to the display, otherwise you override the value old value.
### Basic usage
Download the library and put it in the library folder. Include it in your sketch, create the object, thats it.
```c
#include <LX_Lcd_Varoio.h>
LX_Lcd_Vario lx;
```
Take a look in the example Sketch.
### Mapping
The LCD_VARIO_mapping.h file took me much time to figure out. The mapping of every segment is organized in a 2D Array and sorted in groups of elements. Every Segment consists of three fields. The *first* field describes which controller is responsible for the segment. The *second* field is the pointer and the *third* is the actually data.
#### Functions
##### void addPCF(pcfAddr, modeSet, devSel, blink, bankSel)
This function is a remnant of the attempt to write a universal library for the PCF8576. For now its working only with the initialization of the two PCF's but maybe I hardcode the PCF's sometime to make this library more specific to the LX Lcd Vario Display. You can combine the settings with the predefined bits in LX_LcdVario.h
```c
#define PCFAddress B111000 //This defines the Slave Adress on the I2C Bus, its controlled via the SA0 Pin
byte set_modeset = MODESET | MODE_NORMAL | DISPLAY_ENABLED | BIAS_HALF | DRIVE_4;
byte set_blink = BLINK | BLINKING_ALTERNATION | BLINK_FREQUENCY_OFF;
byte set_bankselect = BANKSELECT | BANKSELECT_O1_RAM0 | BANKSELECT_O2_RAM0; //doesn't really matter, because Drive mode is 4
byte set_deviceselect_1 = DEVICE_SELECT; //A0, A1, A2 set to ground
byte set_deviceselect_2 = DEVICE_SELECT | DEVICE_SELECT_A0; //A0 is high, the others low

void setup() {
	//add your controller to the Library.
	pcf.addPCF(B111000, set_modeset, set_deviceselect_1, set_blink, set_bankselect);
	pcf.addPCF(B111000, set_modeset, set_deviceselect_2, set_blink, set_bankselect);
}
```
##### void init()
The init function set up the PCF's and the library. It works with the LX Lcd Vario but have to improved for universal use (soon). Call this one after *addPCF()*
##### void fire()
This function lets every display segment light up.
##### void clear()
This function clears every segment.
##### void addInd(val)
This function takes a value in the unit m/s (meter per second) and displays the corresponding vario indication element.
##### void addScr(val)
This function displays the elements of the speed command ring (scr). For now it takes a value and displays the element like the one in the mapping of the display. I don't know yet for what I want to use this indicator.
##### void addSym(val)
This function writes one of the 21 symbols to the buffer. For now it takes only a number but later on I want to pass a argument like *BAT* or something. You can look up the symbols in the *LCD_VARIO_mapping.h* file.
##### void addNumber(pos, val)
This function writes a number to one of the 9 nummeric fields.
##### void show()
This one is important. The function actually writes the buffer to the display. Only the *fire()* and *clear()* functions working without this one. After writing the buffer to the display the buffer is flushed. My first try (*showOLD()*) was to clear the display first and then write the data but the display starts to flicker. So this one compares the old buffer with the new one and only clear the unused data and write the new.
## ToDo
- [x] Basic features
- [ ] Optimize the number function
- [ ] Code and speed optimization
- [ ] get it work with our OV (RS232, Software, ...)

And much more, if you want to contribute open a ticket, fork it or send me a pull request.
Discussion in the [XCSoar Forum](https://forum.xcsoar.org/viewtopic.php?f=30&t=3335)
