#include "OledDisplay.h"
#include "images.h"

OledDisplay::OledDisplay(const uint8_t adress, int scrollingOfset): 
adress(adress), 
display(adress),
scrollingOfset(scrollingOfset)
{}

void OledDisplay::init(){
    if(!display.init()) Serial.println("failed to initiliaze oled");
    Serial.println("iniliazed oled  from OledDisplay init");


    display.flipScreenVertically();
}
void OledDisplay::setDisplay(){
    display.clear();
    
}

void OledDisplay:: buttonOption(){
    display.setFont(ArialMT_Plain_10);
    display.drawString(50,40,"press A to connect");
    display.drawString(50,51,"press B to directory");

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
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(50,0,"Avaible devices:");
    display.drawLine(0,11,128,11);
    display.setFont(ArialMT_Plain_10);

    if (avaibleDevices.size()==0) display.drawString(50,15,"...");
    

    for(int i=0; i< avaibleDevices.size(); i++){

        int y=(10*i)+ (scrollingOfset * i) + 15;
        
        if (i== index){
            std::string deviceInfo=(avaibleDevices[i].name + avaibleDevices[i].connectionQuality).c_str();
            display.drawString(35,y,">");
            drawText(deviceInfo,y,45,128,7,10);
            //display.drawString(45,(y + 10*i)+ (scrollingOfset * i),">  " + avaibleDevices[i].name + avaibleDevices[i].connectionQuality);
        }
        else{
            
            // drawText(deviceInfo,y,35,128,7,10);
            display.drawString(35,y,avaibleDevices[i].name + avaibleDevices[i].connectionQuality);
        }
    }
    display.display();

}

void OledDisplay::showConnecting(BluetoothDevice device){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    
    display.setFont(ArialMT_Plain_10);
    display.drawString(10,2,"conecting..");
    display.drawString(10,10,device.name);

    display.setFont(ArialMT_Plain_10);
    display.drawString(10,25,device.connectionQuality + "");
    display.display();
}

void OledDisplay:: showConnectionSucces(BluetoothDevice device){

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,10,"connected to"+device.name);

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
    display.drawString(10,00,"ERROR failed connection to "+device.name);

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

        int y= 10 * i + 15+(scrollingOfset* i);

        if (i== index){
            display.drawString(40,y," >");
            drawText((String(dir.files[i].name())).c_str(),y,45,128,7,10);
        }
        else{
            
            display.drawString(45,y," > "+String(dir.files[i].name()));
        }
    }
    showConnectionState(isConnected);
    

}

void OledDisplay::showDisconnected(){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(15,0,"device is disconnected");
    
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

void OledDisplay::slideText(std::string text,int y,int start_x, int end_x, int char_size =10){
    
    slideOffset +=2;
    for (int i =0; i< text.size(); i++){
      
        int x= start_x+(i *char_size) +  slideOffset;
        
        if (i== 0 && x >= end_x) {slideOffset =1; Serial.println("last char on end");}
        else if (x >= end_x) x= x - end_x + start_x; 
        
        display.drawString(x,y, String(text[i]));
        display.display();
     
        
        
        
    }   
    display.clear();
}

void OledDisplay ::drawText(std::string text,int y,int start_x, int end_x,int char_size, int thresholdLen){
    if (text.size() >= thresholdLen) slideText(text,y,start_x,end_x,char_size); Serial.println(text.size()); return;
    display.drawString(start_x,y,text.c_str()); 
    Serial.println("size is smaller");
    
}
