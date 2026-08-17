
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
        PlayerManager(AudioProcessor* audioProcessor, Selected* selected, CurrentDirectory* currentDirectory);
        void startAudio(String filePath);
        void play();
        void pause();
        void stop();
        void next();
        void previous();
    private:
        AudioProcessor* audioProcessor;
        DirectoryNavigater* directoryNavigater;
        PlayerStates state; 
        Selected* selected;
        CurrentDirectory* currentDirectory;
};

#endif