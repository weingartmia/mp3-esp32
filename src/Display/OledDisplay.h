#pragma once
#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include "DFRobot_OLED12864.h"
#include "BluetoothHandling/BluetoothHandling.h"

class OledDisplay{
    public:
        OledDisplay(const uint8_t adress,int scrollingOfset);
        void init();
        void setDisplay();

        void showLoadingBegin();
        void showAvaibleDevices(std::vector<BluetoothDevice> avaibleDevices,int index );

        void showConnecting(BluetoothDevice device);
        void showConnectionSucces(BluetoothDevice device);
        void showConnectionFail(BluetoothDevice device);

        void showDisconnected();
        void showDisconnecting();
        void showErrorScreen();
        void showConnectionState(bool isConnected);

        void drawDirectory(CurrentDirectory dir, int index,bool isConnected);
        void showPlaying(String songName, String context, String currentTime, String totalTime, int progress, int volume);
        void showVolume( int volume);

        


        
    private:
        DFRobot_OLED12864 display;
        const uint8_t adress;
        int scrollingOfset;
        void buttonOption();
        void slideText(std::string text,int y,int start_x, int end_x,int char_size);
        void drawText(std::string text,int y,int start_x, int end_x,int char_size, int thresholdLen);
        int slideOffset = 1;
};
#endif