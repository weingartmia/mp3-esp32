#include "AudioProcessor.h"

AudioProcessor * AudioProcessor::instance= nullptr;

AudioProcessor::AudioProcessor(const uint8_t _csSDPin): 
_csSDPin(_csSDPin), 
decoder(&_currentFile,&mp3),
metaCopier(out, _currentFile)
{
    instance=this;
};

void printMetaData(MetaDataType type, const char* str, int len){
    
    
    Serial.print("==> ");
    Serial.print(toStr(type));
    if (!str|| str == nullptr){
        AudioProcessor::instance->metadata.album= "";
        AudioProcessor::instance->metadata.artist="";
        AudioProcessor::instance->metadata.title="";

    }
    else if (toStr(type) == "Album") AudioProcessor::instance->metadata.album = String(str);
    else if (toStr(type) == "Artist") AudioProcessor::instance->metadata.artist = String(str);
    else if (toStr(type) == "title") AudioProcessor::instance->metadata.title = String(str);


    Serial.print(": ");
    Serial.println(str);
    
    
}

bool AudioProcessor::init(){
    
    Serial.println("init from audio processor");
    SPI.begin(18,19,23,_csSDPin);
    
  if (!SD.begin(_csSDPin)) {
    Serial.println("SD CARD FAILED, OR NOT PRESENT!");
    // while (1); 
    return false;
  }
  
  out.add(outMeta);
 

  outMeta.setCallback(printMetaData);
  outMeta.begin();
  return true;
  
}

bool AudioProcessor::openFile(const String& filepath){

//    closeCurrentFile();
    _paused= false;
    if (_currentFile){
    _currentFile.close();
    playedFrames=0;
    }

    _currentFile= SD.open(filepath);

    if(!_currentFile){
        Serial.println("failed to open file");
        return false;
    } 
    metaCopier.copy();
        
    processFrame();
    _currentFile.seek(0);

    decoder.transformationReader().resizeResultQueue(1024 * 8);
    if (!decoder.begin()) {
        Serial.println("Decoder begin() failed");
        _currentFile.close();
        return false;
    }
    
    return true;
}

void AudioProcessor::closeCurrentFile(){
    if (_currentFile){

        playedFrames =0;
        _currentFile.close();
        Serial.println("closed current");
        
    }
    decoder.end();

}

int32_t AudioProcessor::readAudio(uint8_t* buffer, int32_t len){

    if (_paused){ // processor is paused
        memset(buffer,0,len);
        return len;
    }
    int32_t bytes_read = decoder.readBytes(buffer, len);
    
    playedFrames += bytes_read / (channels * sizeof(int16_t));// frames are diivided by number of channels and how many bytes fit into each channel

    if (bytes_read < len) {
        memset(buffer + bytes_read, 0, len - bytes_read);
    }

    
    return len;

}

double AudioProcessor::getCurrentTime(){
    return static_cast<double>(playedFrames) / _header.sampleRate;

}

double AudioProcessor::getTotalTime(){

    return static_cast<double>(_header.frames * _header.samplesPerFrame) / _header.sampleRate;

}

void AudioProcessor::pauseCurrentFile(){
    _paused = true;
}

void AudioProcessor::playCurrentFile(){
    _paused = false;
}



bool AudioProcessor::songHasEnded(){

    if ((!_currentFile || !_currentFile.available()) && !_paused ){// when audio finishes  there is no more decoding at the moment
        Serial.println("Song has ended");
        return true;
    }
    return false;
    
}

uint32_t AudioProcessor::getXingOffset(int8_t version,uint32_t channelMode ){
    Serial.printf("channel mode: %d\n", channelMode);
    uint32_t xingOffset;
    if (version == 3) {              // MPEG-1
        xingOffset =  4 + (channelMode == 3 ? 17 : 32);
        } 
    else {                         // MPEG-2 / 2.5
    xingOffset =  4 + (channelMode == 3 ? 9 : 17);
    }
    return xingOffset;

}
 void AudioProcessor:: processFrame(){

    uint32_t samplesPerFrame=0;
    uint32_t bitrate=0;// bits per second
    uint32_t frameCount=0;

    uint8_t h[4];
 
    _currentFile.seek(0);
    _header = {};
    uint8_t id3[10];

if (_currentFile.read(id3, 10) == 10 && id3[0] == 'I' && id3[1] == 'D' && id3[2] == '3') {

    uint32_t tagSize =
    ((uint32_t)(id3[6] & 0x7F) << 21) |
        ((uint32_t)(id3[7] & 0x7F) << 14) |
        ((uint32_t)(id3[8] & 0x7F) << 7)  |
        ((uint32_t)(id3[9] & 0x7F));

    uint32_t audioStart = 10 + tagSize;

    Serial.printf("ID3 size: %lu\n", tagSize);
    Serial.printf("Audio starts around: %lu\n", audioStart);


    _currentFile.seek(audioStart);

}
 

    uint32_t headerPosition = _currentFile.position();

    if (_currentFile.read(h, 4) != 4){
            Serial.println("-----end-------"); 
            // break;
    }
    
        if (h[0] != 0xFF || (h[1] & 0xE0) != 0xE0){
            Serial.println("-----bad beggining-------");
            _currentFile.seek(headerPosition+1);
            
        }


        int8_t version = (h[1] >> 3) & 0x03;// mpeg version
        uint8_t layer = (h[1] >> 1) & 0x03; // layer - hoe is audio encoded            
        uint8_t bitrateIndex = (h[2] >> 4) & 0x0F;//  bitrate index
        uint8_t sr = (h[2] >> 2) & 0x03;//sample-rate index
        uint8_t padding = (h[2] >> 1) & 0x01; // when is audio encoded, length of bytes may become float, padding tells if there is one extra byte
        uint8_t channelMode = (h[3] >> 6) & 0x03;
    
        if (layer != 1){ // target is layer |||.
        _currentFile.seek(headerPosition+1);
        Serial.println("------layer isnt 3-----" + String(layer));
        // continue;
    }
        // reserved version
        if (version == 1){
        Serial.println("-------verion is reversed------");
        // continue;
    }

        // invalid bitrate/sample-rate indexes
        if (bitrateIndex == 0 || bitrateIndex == 15 ||sr == 3){
            _currentFile.seek(headerPosition+1);
            Serial.println("------invalide bitrate-------");
            // continue;
        }
        const uint16_t bitrates[] = {
            0, 32, 40, 48, 56, 64, 80, 96,
            112, 128, 160, 192, 224, 256, 320,0
        };  
        
        bitrate= bitrates[bitrateIndex];
        uint32_t frameSize; 
        
            if (version==3) { // mpeg-1
                const uint32_t sampleRatesMPEG1[] = {44100,48000,32000}; // only for version 3
                sampleRate= sampleRatesMPEG1[sr];
                samplesPerFrame = 1152;   
                frameSize = (144* bitrate * 1000) / sampleRate + padding;
            }
            else if ( version==2) { // mpeg-2
                const uint32_t sampleRatesMPEG1[] = {22050, 24000, 16000}; // only for version 2
                sampleRate= sampleRatesMPEG1[sr];
                samplesPerFrame = 576;
                frameSize = (72 * bitrate * 1000) / sampleRate + padding;
            }
            else if (version==0){// mpeg-2.5
                const uint32_t sampleRatesMPEG1[] = { 11025, 12000, 8000}; // only for version 0
                sampleRate= sampleRatesMPEG1[sr];
                 
                samplesPerFrame = 576;
                frameSize = (72 * bitrate * 1000) / sampleRate + padding;
            }

    _currentFile.seek(getXingOffset(version,channelMode) + headerPosition);

    char tag[5];

    if (_currentFile.read((uint8_t*)tag, 4) == 4) {
        Serial.printf(
        "Tag: %c%c%c%c\n",
        tag[0], tag[1], tag[2], tag[3]
    );

    uint32_t flags =
    ((uint32_t)_currentFile.read() << 24) |
    ((uint32_t)_currentFile.read() << 16) |
    ((uint32_t)_currentFile.read() << 8)  |
    (uint32_t)_currentFile.read();

    if (flags & 0x01) {
                frameCount =
        ((uint32_t)_currentFile.read() << 24) |
        ((uint32_t)_currentFile.read() << 16) |
        ((uint32_t)_currentFile.read() << 8)  |
        (uint32_t)_currentFile.read();

    Serial.printf("Xing frame count: %lu\n", frameCount);
}
}
    _header.samplesPerFrame=samplesPerFrame;
    _header.sampleRate= sampleRate;
    _header.frames= frameCount;

    Serial.println(frameCount);
    
}



