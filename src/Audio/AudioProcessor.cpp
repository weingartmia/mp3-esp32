#include "AudioProcessor.h"

AudioProcessor::AudioProcessor(const uint8_t _csSDPin): _csSDPin(_csSDPin), decoder(&_currentFile,&mp3){};


void AudioProcessor::init(){
    
  if (!SD.begin(_csSDPin)) {
    Serial.println("SD CARD FAILED, OR NOT PRESENT!");
    while (1); 
  }
}

bool AudioProcessor::openFile(const String& filepath){

    closeCurrentFile();
    _currentFile= SD.open(filepath);

    if(!_currentFile){
        Serial.println("failed to open file");
        return false;
    } 
    decoder.transformationReader().resizeResultQueue(1024 * 8);
    if (!decoder.begin()) {
        Serial.println("Decoder begin() failed");
        _currentFile.close();
        return false;
    }
    
    Serial.printf("Opened file: %s\n", filepath.c_str());
    return true;
}

void AudioProcessor::closeCurrentFile(){
    if (_currentFile){
        _currentFile.close();
    }

}

int32_t AudioProcessor::readAudio(uint8_t* buffer, int32_t len){

    if (!_currentFile|| !_currentFile.available()){// when audio finishes  there is no more decoding at the moment
        return 0;
    }

    int32_t bytes_read = decoder.readBytes(buffer, len);

    if (bytes_read < len) {
        memset(buffer + bytes_read, 0, len - bytes_read);
    }
    
    return len;

}