#pragma once

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Audio/AudioProcessor.h"
#include "Directory/DirectoryNavigater.h"

enum class PlayerStates{
    PLAYING,
    PAUSED,
    STOPPED
};

class PlayerManager{
    public:
        PlayerManager(AudioProcessor* audioProcessor,DirectoryNavigater* navigater);
        void startAudio(String filePath);
        void togglePlayStop();
        void play();
        void pause(); // just for atdp external commands
        void stop();
        void next();
        void previous();
        void onSongEnded();
        bool isPlaying();
    private:
        AudioProcessor* audioProcessor;
        DirectoryNavigater* navigater;
        PlayerStates state; 
        // Selected* selected;
        // CurrentDirectory* currentDirectory;
        // std::string* parentDirectory;
};

#endif