
#include "States.h"
#include "utils.h"

State::State(): 
processor(SD_CARD_CS), 
navigater(ROOT),
player(&processor, &navigater.selected, &navigater.currentDirectory),
bluetooth(&processor, &player),
buttons(KEY_A_BUTTON,KEY_B_BUTTON,KEY_ANALOG),
display(0x3c, SCROLLING_OFFSET)

{   

    processor.init();
    bluetooth.init("mp3-esp32");
    buttons.init();
    display.init();
    state =this;
}

void State::setState(State* state){
    this->state= state;
}

void ConnectionChangeState::handleConnectionChange(){

   switch(bluetooth.status){
        case BluetoothStatus::DISCONNECTED:
            Serial.println("device is disconnected from ConnectionState()");
            display.showLoadingBegin();
            break;
        case BluetoothStatus::DISCOVERING:
            Serial.println("device is discovering from ConnectionState()");
            display.showAvaibleDevices(bluetooth.avaibleDevices,bluetooth.index);
            break;
        case BluetoothStatus::CONNECTING:
            Serial.println("device is connecting from ConnectionState()");
            display.showConnecting(bluetooth.avaibleDevices[bluetooth.index]);
            break;
        case BluetoothStatus::CONNECTED:
            Serial.println("device is connected from ConnectionState()");
            this->state->onConnected();
            break;
        case BluetoothStatus::DISCONNECTING:
            Serial.println("device is disconnected from ConnectionState()");
            display.showDisconnecting();
            break;
        case BluetoothStatus::ERRORCONNECTING:
            Serial.println("error while trying to connect from ConnectionState()");
            display.showConnectionFail(bluetooth.avaibleDevices[bluetooth.index]);
            setState(new ErrorState());

    }
    
}

void ConnectionChangeState:: passiveConnection(){
    bool isConnected= bluetooth.status== BluetoothStatus::CONNECTED;
    display.showConnectionState(isConnected);

}

void ConnectionState::handleInputs(){

    buttons.onButtonEvent([this]() {bluetooth.startDiscovering();},KEY_A_PRESS);
    buttons.onButtonEvent([this]() {bluetooth.stopDiscovering();},KEY_A_HOLD);

    buttons.onButtonEvent([this]() {bluetooth.connect();},KEY_B_PRESS);
    buttons.onButtonEvent([this]() {bluetooth.disconnect();},KEY_B_HOLD);

    buttons.onAnalogEvent([this]() {bluetooth.decreaseIndex();},KEY_ANALOG_UP);
    buttons.onAnalogEvent([this]() {bluetooth.increaseIndex();},KEY_ANALOG_DOWN);

}

void ConnectionState::handleAction(){
    handleInputs();
    handleConnectionChange();

}
void ConnectionState::onConnected(){
    
    display.showConnectionSucces(bluetooth.avaibleDevices[bluetooth.index]);
    delay(1000);
    setState(new SelectingState());

}

void ErrorState:: onConnected(){
    passiveConnection();
}
void ErrorState::handleInputs(){
    buttons.onButtonEvent([this](){setState(new ConnectionState());}, KEY_A_PRESS);
    buttons.onButtonEvent([this](){setState(new SelectingState());}, KEY_B_PRESS);

}
void ErrorState::handleAction(){
    handleInputs();
    display.showErrorScreen();
}

void LoadingState::handleInputs(){
    buttons.onButtonEvent([this](){setState(new ConnectionState());}, KEY_A_PRESS);
    buttons.onButtonEvent([this](){setState(new SelectingState());}, KEY_B_PRESS);
}
void LoadingState::handleAction(){
    handleInputs();
    display.showLoadingBegin();

}


void SelectingState::handleInputs(){

    buttons.onButtonEvent([this](){bluetooth.volumeUp();}, KEY_A_HOLD);
    buttons.onButtonEvent([this](){bluetooth.volumeDown();}, KEY_B_HOLD);
    
    buttons.onAnalogEvent([this](){navigater.increaseSelected();}, KEY_ANALOG_DOWN);
    buttons.onAnalogEvent([this](){navigater.decreaseSelected();}, KEY_ANALOG_UP);
    buttons.onAnalogEvent([this](){navigater.openNextDirectory();}, KEY_ANALOG_RIGHT);
    buttons.onAnalogEvent([this](){navigater.exitDirectory();}, KEY_ANALOG_LEFT);

}

void SelectingState ::onConnected(){
    passiveConnection();
}
void SelectingState::handleAction(){
    handleInputs();
    handleConnectionChange();
    display.drawDirectory(navigater.currentDirectory, navigater.selected.index);

}