#include "ButtonHandling.h"
#include "utils.h"
#include <Arduino.h>

ButtonHandling::ButtonHandling(const uint8_t keyA,const uint8_t keyB, const uint8_t analogKey): 
keyA(keyA),keyB(keyB),analogKey(analogKey),Time(millis()),interval(Time),timer(Time)
{

}

void ButtonHandling::init(){

    pinMode(keyA,INPUT);
    pinMode(keyB,INPUT);
    pinMode(analogKey,INPUT);
    Serial.println("initiliazed buttons from ButtonHandling init");
    
}


void ButtonHandling:: setOnkeyEnter(bool& onKeyEnter){
  if (onKeyEnter) {
    timer=interval;
    onKeyEnter = false;
    Serial.println("resseted timer");
    }
}

int  ButtonHandling:: handleButtonPress(bool& onKeyEnter, uint8_t pin){

  
    
    if ( interval- timer <=PRESS_TIME && digitalRead(pin)==1){

      Serial.println("short press");
      onKeyEnter=true;
      return  2;
    }
   else if( interval- timer >PRESS_TIME && digitalRead(pin)==1) {
 
      Serial.println("long hold");
      onKeyEnter=true; 
      return 1;
  }
  else  return 0;
 
    
}
ButtonKeys ButtonHandling::getButtonValue(){

    Time= millis();
    interval= Time;

    bool isPressedA= digitalRead(keyA) ==0;
    bool isPressedB = digitalRead(keyB) ==0;
  

    if (isPressedA ) {
        Serial.println("Button A pressed");
        setOnkeyEnter(onKeyEnterA);     
    }
    if (!onKeyEnterA){

      if (handleButtonPress(onKeyEnterA,keyA)==2) return KEY_A_PRESS;
      else if (handleButtonPress(onKeyEnterA, keyA) ==1)  return KEY_A_HOLD;
    }
    if (isPressedB ) {
        // Serial.println("Button B pressed");
        setOnkeyEnter(onKeyEnterB);
    }
    if (!onKeyEnterB){
      if (handleButtonPress(onKeyEnterB,keyB)==2) return KEY_B_PRESS;
      else if (handleButtonPress(onKeyEnterB, keyB) ==1) return KEY_B_HOLD;
    }
    
    return NO_KEY;

}

AnalogKeys ButtonHandling:: getAnalogValue(){

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

void ButtonHandling:: onButtonEvent(std::function<void()> callback, ButtonKeys key){
    if (getButtonValue() == key){
        Serial.println("button event recieved");
        callback();
    }

}

void ButtonHandling:: onAnalogEvent(std::function<void()> callback, AnalogKeys key){
    if(getAnalogValue() ==key){
        Serial.println("analog event recieved");
        callback();
    }
}