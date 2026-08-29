
#include "States.h"
#include "utils.h"

State::State(): 
// processor(SD_CARD_CS), 
// navigater(ROOT),
// player(&processor, &navigater),
// bluetooth(&processor, &player),
buttons(KEY_A_BUTTON,KEY_B_BUTTON,KEY_ANALOG),
display(0x3c, SCROLLING_OFFSET)
{   
    // processor.init();
    bluetooth.init("mp3-esp32");
    buttons.init();
    display.init();
    state =this;
    Serial.println("State constructor succesfull");
}



void State::setState(State* state){
    
    this->state= state;
    Serial.print("--------state transition--------");
}

void ConnectionChangeState::handleConnectionChange(){

   switch(bluetooth.status){
        case BluetoothStatus::DISCONNECTED:
            Serial.println("device is disconnected from ConnectionState()");
            this->state->onDisconnected();
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
            break;

    }
    
}

void ConnectionChangeState:: passiveConnection(){
    isConnected= bluetooth.status== BluetoothStatus::CONNECTED;
    Serial.println(isConnected);
}

void ConnectionState::handleInputs(){

    buttons.onButtonEvent([this]() {bluetooth.startDiscovering();},KEY_A_HOLD);
    // buttons.onButtonEvent([this]() {bluetooth.stopDiscovering();},KEY_A_HOLD);

    // buttons.onButtonEvent([this]() {setState(new SelectingState());},KEY_B_PRESS);

    buttons.onAnalogEvent([this]() {bluetooth.connect();},KEY_ANALOG_RIGHT);
    buttons.onAnalogEvent([this]() {bluetooth.disconnect();},KEY_ANALOG_LEFT);

    buttons.onAnalogEvent([this]() {bluetooth.decreaseIndex();},KEY_ANALOG_UP);
    buttons.onAnalogEvent([this]() {bluetooth.increaseIndex();},KEY_ANALOG_DOWN);

}

void ConnectionState::handleAction(){
    Serial.println("in connection state --- handling action");
    handleInputs();
    handleConnectionChange();

}
void ConnectionState::onConnected(){
    
    display.showConnectionSucces(bluetooth.avaibleDevices[bluetooth.index]);
    delay(1000);
    // setState(new SelectingState());

}
void ConnectionState::onDisconnected(){
    display.showDisconnected();

}

void ErrorState:: onConnected(){
    passiveConnection();
}
void ErrorState:: onDisconnected(){
    passiveConnection();
}
void ErrorState::handleInputs(){
    buttons.onButtonEvent([this](){setState(new ConnectionState());}, KEY_A_PRESS);
    // buttons.onButtonEvent([this](){setState(new SelectingState());}, KEY_B_PRESS);

}
void ErrorState::handleAction(){
    handleInputs();
    display.showErrorScreen();
}

void LoadingState::handleInputs(){
    buttons.onButtonEvent([this](){setState(new ConnectionState());}, KEY_A_PRESS);
    // buttons.onButtonEvent([this](){setState(new SelectingState());}, KEY_B_PRESS);
}
void LoadingState::handleAction(){
    handleInputs();
    display.showLoadingBegin();

}


// void SelectingState::handleInputs(){

//     buttons.onButtonEvent([this](){setState(new PlayingState()); player.play();}, KEY_A_PRESS);
//     buttons.onButtonEvent([this](){setState(new ConnectionState());}, KEY_B_PRESS);

//     buttons.onButtonEvent([this](){bluetooth.volumeUp();}, KEY_A_HOLD);
//     buttons.onButtonEvent([this](){bluetooth.volumeDown();}, KEY_B_HOLD);
    
//     buttons.onAnalogEvent([this](){navigater.increaseSelected();}, KEY_ANALOG_DOWN);
//     buttons.onAnalogEvent([this](){navigater.decreaseSelected();}, KEY_ANALOG_UP);
//     buttons.onAnalogEvent([this](){navigater.openNextDirectory();}, KEY_ANALOG_RIGHT);
//     buttons.onAnalogEvent([this](){navigater.exitDirectory();}, KEY_ANALOG_LEFT);

// }

// void SelectingState ::onConnected(){
//     passiveConnection();
// }

// void SelectingState:: onDisconnected(){
//     passiveConnection();
// }

// void SelectingState::handleAction(){
//     handleInputs();
//     handleConnectionChange();
//     display.drawDirectory(navigater.currentDirectory, navigater.selected.index,isConnected);

// }


// void PlayingState:: handleInputs(){

//     buttons.onButtonEvent([this](){player.play();}, KEY_A_PRESS); // or pause
//     buttons.onButtonEvent([this](){player.stop();setState(new SelectingState());}, KEY_B_PRESS);

//     buttons.onButtonEvent([this](){bluetooth.volumeUp();}, KEY_A_HOLD);
//     buttons.onButtonEvent([this](){bluetooth.volumeDown();}, KEY_B_HOLD);
    
//     // buttons.onAnalogEvent([this](){player();}, KEY_ANALOG_DOWN);
//     // buttons.onAnalogEvent([this](){navigater.decrea();}, KEY_ANALOG_UP);
//     buttons.onAnalogEvent([this](){player.next();}, KEY_ANALOG_RIGHT);
//     buttons.onAnalogEvent([this](){player.previous();}, KEY_ANALOG_LEFT);

// }


// void PlayingState::onConnected(){
//     Serial.println("connected");
// }
// void PlayingState::onDisconnected(){
//     Serial.println("connection lost...");
//     setState(new ConnectionState());
// }

// String PlayingState:: convertToMinutes(double time){

//     int minutes = time/ 60;
//     int seconds = std::fmod(time,60);

//     return String(minutes) + ":"+ String(seconds);


// }

// void PlayingState::handleAction(){
    // handleInputs();
    // handleConnectionChange();

    // if (navigater.selected.index != _playedSongIndex){
    //     _playedSongIndex = navigater.selected.index;
    //     _totalTime=processor.getMP3Duration(navigater.currentDirectory.files[_playedSongIndex]);
    // }
    // double currentTime= processor.getCurrentTime();

    // String songName = navigater.currentDirectory.files[navigater.selected.index].name(); // current selected song
    // String context; // metadata author, album
    // String current = convertToMinutes(currentTime); //current time in minutes
    // String total = convertToMinutes(_totalTime); // total time   

    // int proggres= currentTime/ _totalTime; //progress time
    // int volume = bluetooth.currentVolume; // current volume
   
    // if (navigater.dontHaveAlbum()) context = String(navigater.parentDirectory.c_str()); // doesnt have album
    // else String context = String(navigater.parentDirectory.c_str()) + " - " +navigater.currentDirectory.name;
    

//     display.showPlaying(songName,context,current,total,proggres,volume);
    
// }