#include "OledDisplay.h"
#include "images.h"

OledDisplay::OledDisplay(const uint8_t adress, int scrollingOfset): 
adress(adress), 
display(adress),
scrollingOfset(scrollingOfset)
{}

void OledDisplay::init(){
    if(!display.init()) Serial.println("failed to initiliaze oled");


    display.flipScreenVertically();
}
void OledDisplay::setDisplay(){
    display.clear();
    
}

void OledDisplay:: buttonOption(){
    display.setFont(ArialMT_Plain_10);
    display.drawString(30,40,"press A to connect");
    display.drawString(30,51,"press B to directory");

}

void OledDisplay:: showLoadingBegin(){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(30,15,"Mp3 player");
    buttonOption();


    display.display();

}

void OledDisplay::showAvaibleDevices(std::vector<BluetoothDevice> avaibleDevices,int index ){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(3,3,"Avaible devices:");

    display.setFont(ArialMT_Plain_10);

    for(int i=0; i< avaibleDevices.size(); i++){
        if (i== index){
            display.drawString(3,(6*i)+ (scrollingOfset* index),"<" + avaibleDevices[i].name + avaibleDevices[i].connectionQuality);
        }
        else{
            display.drawString(3,(6*i) + + (scrollingOfset* index),avaibleDevices[i].name + avaibleDevices[i].connectionQuality);
        }
    }
    display.display();

}

void OledDisplay::showConnecting(BluetoothDevice device){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    
    display.setFont(ArialMT_Plain_24);
    display.drawString(10,2,"conecting..");
    display.drawString(10,10,device.name);

    display.setFont(ArialMT_Plain_16);
    display.drawString(10,25,device.connectionQuality + "");
    display.display();
}

void OledDisplay:: showConnectionSucces(BluetoothDevice device){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(10,10,"connected to"+device.name);

    display.display();

}

void OledDisplay:: showDisconnecting(){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(10,10,"Disconnecting...");

    display.display();

}

void OledDisplay:: showConnectionFail(BluetoothDevice device){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(10,00,"ERROR failed connection  to"+device.name);

    display.display();
}

void OledDisplay::showErrorScreen(){
    display.setTextAlignment(TEXT_ALIGN_CENTER);

    buttonOption();
    display.display();

}

void OledDisplay:: showConnectionState(bool isConnected){
    
    if (isConnected) display.drawCircle(1,1,3);
    else display.fillCircle(1,1,3);
    display.display();
}

void OledDisplay::drawDirectory(CurrentDirectory dir, int index, bool isConnected){
    display.clear();
    
    for(int i=0; i< dir.files.size(); i++){
        if (i== index){
            display.drawString(3,(6*i)+ (scrollingOfset* index),"  <|" + String(dir.files[i].name()));
        }
        else{
            
            display.drawString(3,(6*i) + + (scrollingOfset* index)," < "+String(dir.files[i].name()));
        }
    }
    showConnectionState(isConnected);
    

}

void OledDisplay::showDisconnected(){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(10,0,"device is disconnected");
    
    buttonOption();
    display.display();

}

void OledDisplay::showPlaying(String songName, String context, String currentTime, String totalTime, int progress,int volume){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(10,10, songName);

    display.setFont(ArialMT_Plain_10);
    display.drawString(10,30, context);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0,50, currentTime +" / " + totalTime);

    display.drawProgressBar(0,50,128,10,progress);
    showVolume(volume);
}

void OledDisplay::showVolume(int volume){

    display.drawProgressBar(20,5,60,5,volume);
    display.display();


}