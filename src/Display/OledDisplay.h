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

        void showDisconnecting();
        void showErrorScreen();
        void showConnectionState(bool isConnected);

        void drawDirectory(CurrentDirectory dir, int index);


        
    private:
        DFRobot_OLED12864 display;
        const uint8_t adress;
        int scrollingOfset;
        void buttonOption();

};
#endif