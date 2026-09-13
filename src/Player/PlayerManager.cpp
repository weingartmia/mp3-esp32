
#include "PlayerManager.h"



PlayerManager::PlayerManager(AudioProcessor* audioProcessor, DirectoryNavigater* navigater ): 
audioProcessor(audioProcessor), 
navigater(navigater)
{
    state= PlayerStates::PLAYING;
}

void PlayerManager:: startAudio(String filePath){
    state= PlayerStates::PLAYING;
    audioProcessor->playCurrentFile();
    if (!audioProcessor->openFile(filePath)){
        Serial.println("failed to play a song from playerManager play()");
        
        return;
    }
   
    state= PlayerStates::PLAYING;
    Serial.println("playing");

}
void PlayerManager::togglePlayStop(){
    if (state==PlayerStates::PLAYING)pause();
    
    else  play();
}
void PlayerManager::play(){

    state= PlayerStates::PLAYING;
    audioProcessor->playCurrentFile();
    
   
}

void PlayerManager::onSongEnded(){
    if(audioProcessor->songHasEnded() && state == PlayerStates::PLAYING){
        Serial.println("song has ended");
        next();
    }

}
void PlayerManager::pause(){

    state=PlayerStates::PAUSED;

    audioProcessor->pauseCurrentFile();
    Serial.println("audio is paused");
    
}
void PlayerManager::stop(){
    if (state!= PlayerStates::STOPPED){
        audioProcessor->closeCurrentFile();
        state= PlayerStates::STOPPED;
        Serial.println("stopped from player manager stop()");

    }
    

}
void PlayerManager::next(){
    navigater->increaseSelected();
    
    String filePath =navigater->returnPath();
    Serial.println("final path: " + filePath);
    startAudio(filePath);

} 

void PlayerManager::previous(){
    navigater->decreaseSelected();
   
    String filePath =navigater->returnPath();
    startAudio(filePath);

}

bool PlayerManager::isPlaying(){
    if (state== PlayerStates::PLAYING) return true;
    return false;
}