#include <Arduino.h>

#define ADC_SECTION 5 //how many dimensions joystick has

#ifdef 					__ets__       //esp8266
#define ADC_BIT 1024
#elif 	defined ESP_PLATFORM  //esp32
#define ADC_BIT	4096
#else
#define ADC_BIT 1024          //other MCU, revise according to actual situation
#endif

#define PRESS_TIME 400 //  miliiseconds press is considered long hold
#define ROOT "/"

#define SD_CARD_CS D5 // sd card pin

#define KEY_A_BUTTON 26// button a pin
#define KEY_B_BUTTON 25 // button b pin
#define KEY_ANALOG 36 // analog pin

#define VOLUME_STEP 8 // step down  step up volume that goes from 0 to 127



