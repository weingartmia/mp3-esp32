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

ButtonKeys ButtonHandling ::analyseButton(bool& onKeyEnter, uint8_t pin, ButtonKeys press, ButtonKeys hold){
    Time= millis();
    interval= Time;

    bool isPressed = digitalRead(pin)==0;
    if (isPressed){
      Serial.println("pressed: "+ pin);
      setOnkeyEnter(onKeyEnter);
    }
    if(!onKeyEnter){
      if (handleButtonPress(onKeyEnter,pin)==2) {return press;}
      else if (handleButtonPress(onKeyEnter, pin) ==1)  {return hold;}

    }
    return NO_KEY;


}
ButtonKeys ButtonHandling::getButtonValue(){

  ButtonKeys A=analyseButton(onKeyEnterA,keyA,KEY_A_PRESS,KEY_A_HOLD);

  ButtonKeys B = analyseButton(onKeyEnterB,keyB,KEY_B_PRESS,KEY_B_HOLD);
  if (A!= NO_KEY) { return A;}
  if (B!= NO_KEY) { return B; }
  if (B== NO_KEY && A==NO_KEY) {return NO_KEY;}
 

}

AnalogKeys ButtonHandling:: getAnalogValue(){

  int rawAnalogValue = analogRead(analogKey);

  if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 1) / (ADC_SECTION * 2)) {
    return KEY_ANALOG_NO;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 3) / (ADC_SECTION * 2)) {
    delay(50);
    return KEY_ANALOG_RIGHT;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 5) / (ADC_SECTION * 2)) {
    delay(50);
    return KEY_ANALOG_CENTER;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 7) / (ADC_SECTION * 2)) {
    delay(50);
    return KEY_ANALOG_UP;
  } else if(rawAnalogValue > ADC_BIT * (ADC_SECTION * 2 - 9) / (ADC_SECTION * 2)) {
    delay(50);
    return KEY_ANALOG_LEFT;
  } else {
    delay(50);
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