#pragma once
#ifndef BUTTONHANDLING_H
#define BUTTONHANDLING_H

// #include <DFRobot_BME280.h>
#include "DFRobot_OLED12864.h"

enum  AnalogKeys {
	KEY_ANALOG_NO,
	KEY_ANALOG_RIGHT,
	KEY_ANALOG_CENTER,
	KEY_ANALOG_UP,
	KEY_ANALOG_LEFT,
	KEY_ANALOG_DOWN,
};
enum ButtonKeys{
    KEY_A_PRESS,
    KEY_A_HOLD,
    KEY_B_PRESS,
    KEY_B_HOLD,
    NO_KEY
};

class ButtonHandling{
    public:
        ButtonHandling(const uint8_t keyA,const uint8_t keyB, const uint8_t analogKey);

        const uint8_t keyA;
        const uint8_t keyB;
        const uint8_t analogKey;

        bool onKeyEnterA= true;
        bool onKeyEnterB =true;

        void init();
        AnalogKeys getAnalogValue();
        ButtonKeys getButtonValue();
        ButtonKeys analyseButton(bool& onKeyEnter, uint8_t pin,ButtonKeys press, ButtonKeys hold);

        void onButtonEvent(std::function<void()> callback, ButtonKeys key);
        void onAnalogEvent(std::function<void()> callback, AnalogKeys key);
 
    private:
        int timer;
        int interval;
        unsigned long Time;
        int handleButtonPress(bool& onKeyEnter, uint8_t pin);
        void setOnkeyEnter(bool& onKeyEnter);
    

};


#endif