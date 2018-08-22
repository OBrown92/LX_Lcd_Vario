#include <PCF8576.h>

//create the object
PCF8576 pcf;

//Setup the Variables that you have to pass to the library, this works for the LX Vario
#define PCFAddress B111000 //This defines the Slave Adress on the I2C Bus, its controlled via the SA0 Pin
byte set_modeset = MODESET | MODE_NORMAL | DISPLAY_ENABLED | BIAS_HALF | DRIVE_4;
byte set_blink = BLINK | BLINKING_ALTERNATION | BLINK_FREQUENCY_OFF;
byte set_datapointer = LOADDATAPOINTER | 0;
byte set_bankselect = BANKSELECT | BANKSELECT_O1_RAM0 | BANKSELECT_O2_RAM0; //doesn't really matter, because Drive mode is 4
byte set_deviceselect_1 = DEVICE_SELECT; //A0, A1, A2 set to ground
byte set_deviceselect_2 = DEVICE_SELECT | DEVICE_SELECT_A0; //A0 is high


void setup() {
  //It works better with 4V therefore I use it on one of the Arduino Pins
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  delay(100) //time to wake up
	//add your controller to the Library.
	pcf.addPCF(B111000, set_modeset, set_deviceselect_1, set_blink, set_bankselect);
	pcf.addPCF(B111000, set_modeset, set_deviceselect_2, set_blink, set_bankselect);
	//setup the controller
	pcf.init();
}

void loop(){
	//sets all on
	pcf.fire();
	delay(1000);
	
	//clear the display
	pcf.clear();
	delay(1000);

	//Let the indicator go up
	for (float i = -5; i <= 5; i = i + 0.1){
		//the function takes values in m/s
		pcf.addInd(i);
		//Every function, except clear() and fire(), add their display data to a buffer. You have to call show() to bring it to the display
		pcf.show();
		delay(100);
	}

	//Let the Speed Command Ring go up
	for (int i = 0; i < 13; i++){
		//this function takes only numbers of the indicators, for now
		pcf.addScr(i);
		pcf.show();
		delay(100);
	}

	//Write numbers
	for (int i = 1; i < 10; i++){
		//this function writes a number to the selected position, see more in readme
		pcf.addNumber(i, i);
		pcf.show();
		delay(100);
	}

	//Show symbols
	for (int i = 0; i < 22; i++){
		//this function shows the symbols, for now you have to look them up in the mapping.
		pcf.addSym(i);
		pcf.show();
		delay(100);
	}
}