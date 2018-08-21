#include "Arduino.h"
#include "PCF8576.h"
#include <Wire.h>
#include "LCD_VARIO_mapping.h"


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

void PCF8576::show(){
    //show and clear the buffer
    clear();
    for (uint8_t i = 0; i < bufferCount; i++){
        uint8_t aktBuffer[3];
        aktBuffer[0] = buffer[i][0];
        aktBuffer[1] = buffer[i][1];
        aktBuffer[2] = buffer[i][2];      
        for (uint8_t j = 0; j < bufferCount; j++){
            if(aktBuffer[0] == buffer[j][0] && aktBuffer[1] == buffer[j][1]){
                aktBuffer[2] = aktBuffer[2] | buffer[j][2];
            }
        }
        Wire.beginTransmission(settings[0][0]);
        Wire.write(CONTINUE | settings[aktBuffer[0]][2]);   
        Wire.write(aktBuffer[1]); //pointer
        Wire.write(aktBuffer[2]); //data
        Wire.endTransmission();
    }
    //memcpy(buffer, oldBuffer, 256);
    bufferCount = 0;
}

void PCF8576::addToBuffer(uint8_t *val){
    buffer[bufferCount][0] = val[0];
    buffer[bufferCount][1] = val[1];
    buffer[bufferCount][2] = val[2];
    bufferCount++; 
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