#include "Arduino.h"
#include "PCF8576.h"
#include <Wire.h>


//Constructor, initialize Wire lib
PCF8576::PCF8576()
{
    Wire.begin();
}

//Begin method
void PCF8576::begin(){
    
}

void PCF8576::addPCF(uint8_t pcfAddr, uint8_t modeSet, uint8_t devSel, uint8_t blink, uint8_t bankSel){
    settings[PCFcount][0] = pcfAddr;
    settings[PCFcount][1] = modeSet;
    settings[PCFcount][2] = devSel;
    settings[PCFcount][3] = blink;
    settings[PCFcount][4] = bankSel;
    PCFcount++;
}

void PCF8576::init(){
    //init all added PCF's, don't know exactly if you have to init all
    for (uint8_t pcf = 0; pcf < PCFcount; pcf++){
        Serial.println("init");
        Serial.println(settings[pcf][0]);
        Wire.beginTransmission(settings[pcf][0]);
        Wire.write(CONTINUE | settings[pcf][1]); //modeset
        Wire.write(CONTINUE | settings[pcf][3]); //blink
        Wire.write(LAST | settings[pcf][2]); //devSel
        //Wire.write(LAST | settings[pcf][4]); //banksel
        Wire.endTransmission();
    }
}

void PCF8576::lcd_clear(){
    //works only for one Hardware Adress for now, have to implement the logic for two
    Wire.beginTransmission(settings[0][0]);
	Wire.write(CONTINUE | DEVICE_SELECT); //select the first PCF, assume that the first is the device with 000, maybe implement another logic
	Wire.write(LOADDATAPOINTER);
	for (int i=0; i < PCFcount * 20; i++) {
		Wire.write(0x00);
	}
	Wire.endTransmission();
}

void PCF8576::lcd_fire(){
    //same as in lcd_clear()
    Wire.beginTransmission(settings[0][0]);
	Wire.write(CONTINUE | DEVICE_SELECT); //select the first PCF, assume that the first is the device with 000, maybe implement another logic
	Wire.write(LOADDATAPOINTER);
	for (int i=0; i < PCFcount * 20; i++) {
		Wire.write(0xFF);
	}
	Wire.endTransmission();
}