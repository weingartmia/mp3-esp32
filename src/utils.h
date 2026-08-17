#define ADC_SECTION 5 //how many dimensions joystick has

#ifdef 					__ets__       //esp8266
#define ADC_BIT 1024
#elif 	defined ESP_PLATFORM  //esp32
#define ADC_BIT	4096
#else
#define ADC_BIT 1024          //other MCU, revise according to actual situation
#endif

#define PRESS_TIME 100
#define HOLD_TIME 700