#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H
#include "States.h"

#include "Player/PlayerManager.h"
#include "ButtonHandlingLogic/ButtonHandling.h"
#include "Audio/AudioProcessor.h"
#include "BluetoothHandling/BluetoothHandling.h"
#include "Directory/DirectoryNavigater.h"
#include "Display/OledDisplay.h"

class Context{

    private:
        State* state  = nullptr;

    public:
        Context();
        void init();
        void currentStateAction();
        void setState(State* newState);
        void handleConnectionChange();

        AudioProcessor processor;
        ButtonHandling buttons;
        DirectoryNavigater navigater;
        BluetoothManager bluetooth;
        PlayerManager player;
        OledDisplay display;
        
};

#endif