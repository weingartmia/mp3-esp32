#include "AudioProcessor.h"

AudioProcessor::AudioProcessor(const uint8_t _csSDPin): _csSDPin(_csSDPin), decoder(&_currentFile,&mp3){};

void printMetaData(MetaDataType type, const char* str, int len){
    AudioProcessor * proc;
    
    Serial.print("==> ");
    Serial.print(toStr(type));
    proc->metadata = toStr(type);
    Serial.print(": ");
    Serial.println(str);
}

void AudioProcessor::init(){
    
  if (!SD.begin(_csSDPin)) {
    Serial.println("SD CARD FAILED, OR NOT PRESENT!");
    while (1); 
  }
  out.setCallback(printMetaData);
  out.begin();
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

        playedFrames =0;
        _currentFile.close();
        decoder.end();
    }

}

int32_t AudioProcessor::readAudio(uint8_t* buffer, int32_t len){

    // if (!_currentFile|| !_currentFile.available()){// when audio finishes  there is no more decoding at the moment
    //     return 0;
    // }
    if (_paused){ // processor is paused
        memset(buffer,0,len);
        return len;
    }
    int32_t bytes_read = decoder.readBytes(buffer, len);
    playedFrames += bytes_read / (channels * sizeof(int16_t));

    if (bytes_read < len) {
        memset(buffer + bytes_read, 0, len - bytes_read);
    }

    
    return len;

}

double AudioProcessor::getCurrentTime(){
    return static_cast<double>(playedFrames) / sampleRate;

}

void AudioProcessor::pauseCurrentFile(){
    _paused = true;
}

void AudioProcessor::playCurrentFile(){
    _paused = false;
}
double AudioProcessor:: getMP3Duration(File& file){
    const uint32_t start = file.position();

    uint8_t header[4];

    uint64_t totalSamples = 0;
    uint32_t sampleRate = 0;

    while (file.read(header, 4) == 4) {

        
        if (header[0] != 0xFF || (header[1] & 0xE0) != 0xE0) { // ssync word: 11 bits set

            file.seek(file.position() - 3);
            continue;
        }

        int version = (header[1] >> 3) & 0x03;
        int layer   = (header[1] >> 1) & 0x03;
        int bitrateIndex = (header[2] >> 4) & 0x0F;
        int sampleRateIndex = (header[2] >> 2) & 0x03;
        int padding = (header[2] >> 1) & 0x01;

        
        if (layer != 3); 
            return 0;

        if (bitrateIndex == 0 || bitrateIndex == 15 ||sampleRateIndex == 3)
            continue;

        static const int bitrateTableMPEG1[] = {
            0, 32, 40, 48, 56, 64, 80, 96,
            112, 128, 160, 192, 224, 256, 320
        };

        static const int sampleRateTable[] = {
            44100, 48000, 32000
        };

        if (version ==  1) 
            return 0; // mpeg-1 only

        int bitrate =bitrateTableMPEG1[bitrateIndex] * 1000;
        sampleRate =sampleRateTable[sampleRateIndex];

        totalSamples += 1152;

        uint32_t frameLength =
            (144UL * bitrate) / sampleRate + padding;

        if (frameLength < 4)
            break;

        file.seek(file.position() + frameLength - 4);
    }

    file.seek(start);

    if (sampleRate == 0)
        return 0.0;
        

    return static_cast<double>(totalSamples) / sampleRate;
}

bool AudioProcessor::songHasEnded(){

    if (!_currentFile || !_currentFile.available()){// when audio finishes  there is no more decoding at the moment
        return true;
    }
    return false;
    
}


