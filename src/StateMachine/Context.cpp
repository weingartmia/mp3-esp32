#include "Context.h"
#include "utils.h"


Context::Context():
processor(SD_CARD_CS), 
navigater(ROOT),
player(&processor, &navigater),
bluetooth(&processor, &player),
buttons(KEY_A_BUTTON,KEY_B_BUTTON,KEY_ANALOG),
display(0x3c)
{   
  Serial.println("Context constructor succesfull");
}



void Context::init(){

    if (!processor.init()) {display.showInitSDFail();}
    bluetooth.init("mp3-esp32");
    buttons.init();
    display.init();
   
    
    setState(new LoadingState());
    Serial.println("initiliazed context from Context init()");
}

void Context::setState(State* newState){
    
    state= newState;
    state->setContext(this);
    Serial.print("--------state transition--------");
}

void Context::currentStateAction(){
    // Serial.println("----context handling action currrentStateAction()");
    this->state->handleAction();
}


void Context::handleConnectionChange(){

   switch(bluetooth.status){
        case BluetoothStatus::DISCONNECTED:
            // Serial.println("device is disconnected from ConnectionState()");
            state->onDisconnected();
            break;
        case BluetoothStatus::DISCOVERING:

            display.showAvaibleDevices(bluetooth.avaibleDevices,bluetooth.index);
            break;
        case BluetoothStatus::CONNECTING:
            Serial.println("device is connecting from ConnectionState()");
            display.showConnecting(bluetooth.avaibleDevices[bluetooth.index]);
            break;
        case BluetoothStatus::CONNECTED:
  
            state->onConnected();
            break;
        case BluetoothStatus::DISCONNECTING:
            // Serial.println("device is disconnected from ConnectionState()");
            display.showDisconnecting();
            break;
        case BluetoothStatus::ERRORCONNECTING:
            Serial.println("error while trying to connect from ConnectionState()");
            display.showConnectionFail(bluetooth.avaibleDevices[bluetooth.index]);
            setState(new ErrorState());
            break;

    }
    
}

