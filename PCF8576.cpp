#include "Arduino.h"
#include "PCF8576.h"
#include <Wire.h>
#include "LCD_VARIO_mapping.h"


//Constructor, initialize Wire lib
PCF8576::PCF8576()
{
    Wire.begin();
    //oldBuffer[0][0] = 0;
    //oldBuffer[0][1] = 0;
    //oldBuffer[0][2] = 0;
}

//Begin method
void PCF8576::begin(){
    Serial.println(SCR[0][0]);
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
        Serial.println("init complete");
        Wire.beginTransmission(settings[pcf][0]);
        Wire.write(CONTINUE | settings[pcf][1]); //modeset
        Wire.write(CONTINUE | settings[pcf][3]); //blink
        Wire.write(LAST | settings[pcf][2]); //devSel
        //Wire.write(LAST | settings[pcf][4]); //banksel
        Wire.endTransmission();
    }
}

void PCF8576::clear(){
    //works only for one Hardware Adress for now, have to implement the logic for two
    Wire.beginTransmission(settings[0][0]);
	Wire.write(CONTINUE | DEVICE_SELECT); //select the first PCF, assume that the first is the device with 000, maybe implement another logic
	Wire.write(LOADDATAPOINTER);
	for (int i=0; i < PCFcount * 20; i++) {
		Wire.write(0x00);
	}
	Wire.endTransmission();
}

void PCF8576::fire(){
    //same as in lcd_clear()
    Wire.beginTransmission(settings[0][0]);
	Wire.write(CONTINUE | DEVICE_SELECT); //select the first PCF, assume that the first is the device with 000, maybe implement another logic
	Wire.write(LOADDATAPOINTER);
	for (int i=0; i < PCFcount * 20; i++) {
		Wire.write(0xFF);
	}
	Wire.endTransmission();
}

void PCF8576::show2(){
    //because clearing and setting new display data let the display
    //flicker, this is a new try to solve this Problem by not clearing
    //the display first

    //first set all data from the old buffer to zero
    for (uint8_t i = 0; i < oldBufferCount; i++){
        oldBuffer[i][2] = 0;
    }
    //compare if pointer already exist and set data from new buffer
    //if not add line to oldBuffer
    bool found = false;
    for (uint8_t i = 0; i < bufferCount; i++){
        for (uint8_t j = 0; j < oldBufferCount; j++){
            if (buffer[i][1] == oldBuffer[j][1] && buffer[i][0] == oldBuffer[j][0]){
                oldBuffer[j][2] = buffer[i][2];
                found = true;
                //break;
            }
        }
        if(!found){
            
            oldBuffer[oldBufferCount][0] = buffer[i][0];
            oldBuffer[oldBufferCount][1] = buffer[i][1];
            oldBuffer[oldBufferCount][2] = buffer[i][2];
            found = false;
            oldBufferCount++;
        }
    }
    Serial.println(oldBuffer[0][0]);
    Serial.println(oldBuffer[0][1]);
    Serial.println(oldBuffer[0][2]);

    //actually write the data
    for (uint8_t i = 0; i < oldBufferCount; i++){
        Wire.beginTransmission(settings[0][0]);
        Wire.write(CONTINUE | settings[oldBuffer[i][0]][2]); //devsel
        Wire.write(oldBuffer[i][1]); //pointer
        Wire.write(oldBuffer[i][2]); //data
        Wire.endTransmission();
    }


    //copy Array for next interval
    for (uint8_t i = 0; i < bufferCount; i++){
        for (uint8_t j = 0; j < 3; j++){
            oldBuffer[i][j] = buffer[i][j];
        }
    }
    oldBufferCount = bufferCount;
    bufferCount = 0;
}

void PCF8576::show(){
    //clear the display, let it flicker
    clear();
    //Write the data to the display
    for (uint8_t i = 0; i < bufferCount; i++){
        Wire.beginTransmission(settings[0][0]);
        Wire.write(CONTINUE | settings[buffer[i][0]][2]); //devsel
        Wire.write(buffer[i][1]); //pointer
        Wire.write(buffer[i][2]); //data
        Wire.endTransmission();
    }
    //reset the buffer counter, no need to clear the array
    bufferCount = 0;
}

void PCF8576::addToBuffer(uint8_t *val){
    //check if section already set and do bitwise or
    bool found = false;
    for (uint8_t i = 0; i < bufferCount; i++){
        if (buffer[i][0] == val[0] && buffer[i][1] == val[1]){
            buffer[i][2] = buffer[i][2] | val[2];
            found = true;
            break;
        }
    }
    if(!found){
        for (uint8_t i = 0; i < 3; i++){
            buffer[bufferCount][i] = val[i];
        }
        bufferCount++;
    } 
}

void PCF8576::addInd(float val){
    if (val >= 5){
        addToBuffer((uint8_t *)IND[0]);
    }else if (val <= -5){
        addToBuffer((uint8_t *)IND[50]);
    }else{
        val = (uint8_t)round((val - (-5)) * (0 - 50) / (5 - (-5)) + (50));
        addToBuffer((uint8_t *)IND[(uint8_t)val]);
    }
}

void PCF8576::addScr(uint8_t val){
    addToBuffer((uint8_t *)SCR[(uint8_t)val]);
}

void PCF8576::addSym(uint8_t val){
    addToBuffer((uint8_t *)SYM[(uint8_t)val]);
}