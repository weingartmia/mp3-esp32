#include "ButtonHandling.h"
#include "utils.h"
#include <Arduino.h>

ButtonHandling::ButtonHandling(const uint8_t keyA,const uint8_t keyB, const uint8_t analogKey): 
keyA(keyA),keyB(keyB),analogKey(analogKey)
{
timer=0;
}

void ButtonHandling::init(){
    pinMode(keyA,INPUT);
    pinMode(keyB,INPUT);
    pinMode(analogKey,INPUT);
    
}

ButtonKeys  ButtonHandling::getButtonValues(){
    unsigned long Time= millis();
    int interval= Time;


    bool isPressedA= digitalRead(keyA) ==0;
    bool isPressedB = digitalRead(keyB) ==0;

    if (isPressedA && interval- timer >=PRESS_TIME) {return  KEY_A_PRESS; timer=interval;}
    else if (isPressedA && interval- timer >=HOLD_TIME) {return  KEY_A_HOLD; timer=interval;}
    else if (isPressedB && interval- timer >=PRESS_TIME) {return  KEY_B_PRESS; timer=interval;}
    else if (isPressedB && interval- timer >=HOLD_TIME) {return  KEY_B_HOLD; timer=interval;}

}

AnalogKeys ButtonHandling:: getAnalogValues(){

  int rawAnalogValue = analogRead(analogKey);

  if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 1) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_NO;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 3) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_RIGHT;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 5) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_CENTER;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 7) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_UP;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 9) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_LEFT;
  } else {
    return KEY_ANALOG_DOWN;
  }

}