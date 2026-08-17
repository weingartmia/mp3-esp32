
#include "PlayerManager.h"


PlayerManager::PlayerManager(AudioProcessor* audioProcessor, Selected* selected, CurrentDirectory* currentDirectory): audioProcessor(audioProcessor), selected(selected), currentDirectory(currentDirectory)
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

    int currentIndex= selected->index;
    String filePath =currentDirectory->files[currentIndex].name();
    startAudio(filePath);


}
void PlayerManager::pause(){
    state=PlayerStates::PAUSED;
    Serial.println("audio is paused");
    
}
void PlayerManager::stop(){
    audioProcessor->closeCurrentFile();
    state =PlayerStates::STOPPED;
    Serial.println("stopped from player manager stop()");

}
void PlayerManager::next(){
    int currentIndex= selected->index +1;
    String filePath =currentDirectory->files[currentIndex].name();
    startAudio(filePath);

} 

void PlayerManager::previous(){
    int currentIndex= selected->index -1;
    String filePath =currentDirectory->files[currentIndex].name();
    startAudio(filePath);

}