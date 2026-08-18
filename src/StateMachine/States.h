#pragma once 


#ifndef STATES_H
#define STATES_H

#include <Arduino.h>

#include "Player/PlayerManager.h"
#include "ButtonHandlingLogic/ButtonHandling.h"
#include "Audio/AudioProcessor.h"
#include "BluetoothHandling/BluetoothHandling.h"
#include "Directory/DirectoryNavigater.h"
#include "Display/OledDisplay.h"



class State{
    public:
        State();
        void setState(State* state);

        virtual void handleAction() =0 ;
        virtual void handleInputs()=0;
        virtual void onConnected() =0;


        AudioProcessor processor;
        ButtonHandling buttons;
        DirectoryNavigater navigater;
        BluetoothManager bluetooth;
        PlayerManager player;
        OledDisplay display;
        virtual ~State()=default;

        State* state;
};
#endif
class ConnectionChangeState: public State{
    public:
        
        virtual void handleAction() =0 ;
        virtual void handleInputs()=0;
        virtual void onConnected() =0;


        void handleConnectionChange();
        void passiveConnection();

        


        

};
class ConnectionState : public ConnectionChangeState{
    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;


};
class ErrorState : public ConnectionChangeState{
    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;


};

class LoadingState:public State{
    public:
        void handleInputs() override;
        void handleAction() override;

};

class SelectingState: public ConnectionChangeState{
    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;

};
