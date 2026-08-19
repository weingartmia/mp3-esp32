#pragma once

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Audio/AudioProcessor.h"
#include "Directory/DirectoryNavigater.h"

enum class PlayerStates{
    PLAYING,
    STOPPED,
    PAUSED
};

class PlayerManager{
    public:
        PlayerManager(AudioProcessor* audioProcessor,DirectoryNavigater* navigater);
        void startAudio(String filePath);
        void play();
        void pause();
        void stop();
        void next();
        void previous();
    private:
        AudioProcessor* audioProcessor;
        DirectoryNavigater* navigater;
        PlayerStates state; 
        // Selected* selected;
        // CurrentDirectory* currentDirectory;
        // std::string* parentDirectory;
};

#endif