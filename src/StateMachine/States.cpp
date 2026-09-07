#include "Context.h"
#include "States.h"
#include "utils.h"

void State::setContext(Context* newCon){
    con = newCon;
}

void ConnectionChangeState:: passiveConnection(){
    isConnected= con->bluetooth.status== BluetoothStatus::CONNECTED;
    
}

void ConnectionState::handleInputs(){

    con->buttons.onButtonEvent([this]() {con->bluetooth.startDiscovering();},KEY_A_PRESS);
    con->buttons.onButtonEvent([this]() {con->bluetooth.stopDiscovering();},KEY_A_HOLD);

    con->buttons.onButtonEvent([this]() {con->setState(new SelectingState());},KEY_B_PRESS);

    con->buttons.onAnalogEvent([this]() {con->bluetooth.connect();},KEY_ANALOG_RIGHT);
    con->buttons.onAnalogEvent([this]() {con->bluetooth.disconnect();},KEY_ANALOG_LEFT);

    con->buttons.onAnalogEvent([this]() {con->bluetooth.decreaseIndex();},KEY_ANALOG_UP);
    con->buttons.onAnalogEvent([this]() {con->bluetooth.increaseIndex();},KEY_ANALOG_DOWN);

}

void ConnectionState::handleAction(){
    // Serial.println("in connection state --- handling action");
    handleInputs();
    

}
void ConnectionState::onConnected(){
    
    con->display.showConnectionSucces(con->bluetooth.avaibleDevices[con->bluetooth.index]);
    // delay(1000);
    con->setState(new SelectingState());

}
void ConnectionState::onDisconnected(){
    con->display.showDisconnected();

}

void ErrorState:: onConnected(){
    passiveConnection();
}
void ErrorState:: onDisconnected(){
    passiveConnection();
}
void ErrorState::handleInputs(){
    con->buttons.onButtonEvent([this](){con->setState(new ConnectionState());}, KEY_A_PRESS);
    con->buttons.onButtonEvent([this](){con->setState(new SelectingState());}, KEY_B_PRESS);

}
void ErrorState::handleAction(){
    handleInputs();
    con->display.showErrorScreen();
}


void LoadingState::handleInputs(){
    con->buttons.onButtonEvent([this](){con->setState(new SelectingState());}, KEY_A_PRESS);
    //con->buttons.onButtonEvent([this](){con->setState(new SelectingState());}, KEY_B_PRESS);
    
}
void LoadingState::handleAction(){
    handleInputs();
    con->display.showLoadingBegin();

}


void SelectingState::handleInputs(){

    con->buttons.onButtonEvent([this](){con->setState(new PlayingState()); con->player.play();}, KEY_A_PRESS);
    con->buttons.onButtonEvent([this](){con->setState(new LoadingState());}, KEY_B_PRESS);

    con->buttons.onButtonEvent([this](){con->bluetooth.volumeUp();}, KEY_A_HOLD);
    con->buttons.onButtonEvent([this](){con->bluetooth.volumeDown();}, KEY_B_HOLD);
    
    con->buttons.onAnalogEvent([this](){con->navigater.increaseSelected();}, KEY_ANALOG_DOWN);
    con->buttons.onAnalogEvent([this](){con->navigater.decreaseSelected();}, KEY_ANALOG_UP);
    con->buttons.onAnalogEvent([this](){con->navigater.openNextDirectory();}, KEY_ANALOG_RIGHT);
    con->buttons.onAnalogEvent([this](){con->navigater.exitDirectory();}, KEY_ANALOG_LEFT);

}

void SelectingState ::onConnected(){
    passiveConnection();
}

void SelectingState:: onDisconnected(){
    passiveConnection();
}

void SelectingState::handleAction(){
    
    handleInputs();
    
    if (con->navigater.currentDirectory)
        Serial.println("Navigater is nullptr");
        con->navigater.openDirectory(ROOT);
    con->display.drawDirectory(*con->navigater.currentDirectory, con->navigater.selected.index,isConnected);

}


void PlayingState:: handleInputs(){

    con->buttons.onButtonEvent([this](){con->player.play();}, KEY_A_PRESS); // or pause
    con->buttons.onButtonEvent([this](){con->player.stop();con->setState(new SelectingState());}, KEY_B_PRESS);

    con->buttons.onButtonEvent([this](){con->bluetooth.volumeUp();}, KEY_A_HOLD);
    con->buttons.onButtonEvent([this](){con->bluetooth.volumeDown();}, KEY_B_HOLD);
    
    // con->buttons.onAnalogEvent([this](){con->player();}, KEY_ANALOG_DOWN);
    // con->buttons.onAnalogEvent([this](){con->navigater.decrea();}, KEY_ANALOG_UP);
    con->buttons.onAnalogEvent([this](){con->player.next();}, KEY_ANALOG_RIGHT);
    con->buttons.onAnalogEvent([this](){con->player.previous();}, KEY_ANALOG_LEFT);

}


void PlayingState::onConnected(){
    Serial.println("connected");
}
void PlayingState::onDisconnected(){
    Serial.println("connection lost...");
    con->setState(new ConnectionState());
}

String PlayingState:: convertToMinutes(double time){

    int minutes = time/ 60;
    int seconds = std::fmod(time,60);

    return String(minutes) + ":"+ String(seconds);


}

void PlayingState::handleAction(){
    handleInputs();
 

    if (con->navigater.selected.index != _playedSongIndex){
        _playedSongIndex = con->navigater.selected.index;
        _totalTime=con->processor.getMP3Duration(con->navigater.currentDirectory->files[_playedSongIndex]);
    }
    double currentTime= con->processor.getCurrentTime();

    String songName = con->navigater.currentDirectory->files[con->navigater.selected.index].name(); // current selected song
    String context; // metadata author, album
    String current = convertToMinutes(currentTime); //current time in minutes
    String total = convertToMinutes(_totalTime); // total time   

    int proggres= currentTime/ _totalTime; //progress time
    int volume = con->bluetooth.currentVolume; // current volume
   
    if (con->navigater.dontHaveAlbum()) context = String(con->navigater.parentDirectory.c_str()); // doesnt have album
    else String context = String(con->navigater.parentDirectory.c_str()) + " - " +con->navigater.currentDirectory->name;
    

    con->display.showPlaying(songName,context,current,total,proggres,volume);
    
}