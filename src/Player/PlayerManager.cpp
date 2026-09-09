
#include "PlayerManager.h"



PlayerManager::PlayerManager(AudioProcessor* audioProcessor, DirectoryNavigater* navigater ): 
audioProcessor(audioProcessor), 
navigater(navigater)
{
    state= PlayerStates::STOPPED;
}

void PlayerManager:: startAudio(String filePath){

    if (!audioProcessor->openFile(filePath)){
        Serial.println("failed to play a song from playerManager play()");
        
        return;
    }
    Serial.printf("[%s]\n", filePath);
    state= PlayerStates::PLAYING;
    Serial.println("playing");

}
void PlayerManager::play(){
    if (state == PlayerStates::PLAYING) {
        audioProcessor->pauseCurrentFile();
        pause();
        return;
    }

    audioProcessor->playCurrentFile();
    String filePath = navigater->returnPath();
    startAudio(filePath);

    if(audioProcessor->songHasEnded()){
        Serial.println("song has ended");
        next();
    }

}
void PlayerManager::pause(){
    if (state == PlayerStates::PAUSED){ 
        audioProcessor->playCurrentFile();
        play();
        return;
    };

    state=PlayerStates::PAUSED;
    audioProcessor->pauseCurrentFile();
    Serial.println("audio is paused");
    
}
void PlayerManager::stop(){
    audioProcessor->closeCurrentFile();
    state =PlayerStates::STOPPED;
    Serial.println("stopped from player manager stop()");

}
void PlayerManager::next(){
    navigater->increaseSelected();
    int currentIndex = navigater->selected.index;
    String filePath =navigater->currentDirectory->paths[currentIndex];
    Serial.println("final path: " + filePath);
    startAudio(filePath);

} 

void PlayerManager::previous(){
    navigater->decreaseSelected();
    int currentIndex = navigater->selected.index;
    String filePath =navigater->currentDirectory->paths[currentIndex];
    startAudio(filePath);

}