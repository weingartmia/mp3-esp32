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
    
}


bool ButtonHandling:: handleButtonPress(bool onKeyEnter, uint8_t pin){

    interval= Time;
    Serial.print(interval);
    Serial.print(timer);

    if (onKeyEnter) timer=interval; onKeyEnter = false;

    if ( interval- timer <=PRESS_TIME && digitalRead(pin)==1){
        timer=interval;
        onKeyEnter=true;
        return  true;
    }
    return false;
    
}
ButtonKeys  ButtonHandling::getButtonValue(){

    bool isPressedA= digitalRead(keyA) ==0;
    bool isPressedB = digitalRead(keyB) ==0;
    bool onKeyEnterA= true;
    bool onKeyEnterB =true;

    if (isPressedA ) {
        if (handleButtonPress(onKeyEnterA,keyA)) return KEY_A_PRESS;
        else if (!handleButtonPress(onKeyEnterA, keyA)) return KEY_A_HOLD;
    }

    if (isPressedB ) {
        if (handleButtonPress(onKeyEnterB,keyB)) return KEY_B_PRESS;
        else if (handleButtonPress(!onKeyEnterB, keyB)) return KEY_B_HOLD;
    }

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